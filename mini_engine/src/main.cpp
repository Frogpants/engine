#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


enum class BlockKind { Command, Reporter, Boolean };
enum class SlotKind  { Value, Boolean, Statement };

struct Slot {
    SlotKind kind;
    ImVec2 offset;
    ImVec2 size;
    void* attached = nullptr;
    bool editing = false;
    Slot(SlotKind k, ImVec2 off, ImVec2 s) : kind(k), offset(off), size(s) {}
};

struct Block {
    BlockKind kind;
    ImVec2 pos;
    ImVec2 size;
    bool dragging = false;
    ImVec2 dragOffset;

    Block* next = nullptr;
    Block* prev = nullptr;

    std::vector<Slot> slots;
    std::string label;
    bool highlighted = false;

    char valueText[64];

    Block(BlockKind k, const char* lbl, float x, float y) {
        kind = k;
        label = lbl;
        pos = ImVec2(x, y);
        size = (k == BlockKind::Reporter) ? ImVec2(70, 28) : (k==BlockKind::Boolean ? ImVec2(36, 36) : ImVec2(220, 36));
        valueText[0] = '\0';
    }
};

static ImU32 ColorForKind(BlockKind k) {
    switch (k) {
        case BlockKind::Command: return IM_COL32(102,204,255,255);
        case BlockKind::Reporter: return IM_COL32(255,255,255,255);
        case BlockKind::Boolean: return IM_COL32(230,230,230,255);
    }
    return IM_COL32(200,200,200,255);
}
static ImU32 ColorForTypeTag(const std::string &tag) {
    if (tag == "motion") return IM_COL32(46,204,113,255);
    if (tag == "looks")  return IM_COL32(255,193,7,255);
    if (tag == "control") return IM_COL32(155,89,182,255);
    return IM_COL32(120,120,120,255);
}

static bool mouseInRect(const ImVec2& mouse, const ImVec2& p, const ImVec2& s) {
    return mouse.x >= p.x && mouse.x <= p.x + s.x && mouse.y >= p.y && mouse.y <= p.y + s.y;
}

static float dsq(float ax, float ay, float bx, float by) {
    float dx = ax-bx, dy = ay-by;
    return dx*dx + dy*dy;
}


static float clampf(float v, float a, float b) { return std::max(a, std::min(b, v)); }


static void DrawReporter(ImDrawList* dl, ImVec2 p, ImVec2 s, const char* text, bool highlighted=false) {
    ImU32 bg = highlighted ? IM_COL32(255,255,255,200) : IM_COL32(255,225,140,255);
    dl->AddRectFilled(p, ImVec2(p.x+s.x, p.y+s.y), bg, s.y*0.5f);
    dl->AddRect(p, ImVec2(p.x+s.x, p.y+s.y), IM_COL32(0,0,0,80), s.y*0.5f, 0, 1.0f);
    dl->AddText(ImVec2(p.x + 6, p.y + 6), IM_COL32(0,0,0,255), text);
}

void DrawBoolean(ImDrawList* dl, ImVec2 p, ImVec2 s, const char* text, bool state) {
    ImU32 bg = state ? IM_COL32(80, 200, 120, 255) : IM_COL32(200, 80, 80, 255);

    float w = s.x;
    float h = s.y;
    float r = h * 0.5f;

    ImVec2 v0 = ImVec2(p.x + r,     p.y);
    ImVec2 v1 = ImVec2(p.x + w - r, p.y);
    ImVec2 v2 = ImVec2(p.x + w - r, p.y + h);
    ImVec2 v3 = ImVec2(p.x + r,     p.y + h);

    ImVec2 pts[4] = { v0, v1, v2, v3 };

    dl->AddConvexPolyFilled(pts, 4, bg);
    dl->AddPolyline(pts, 4, IM_COL32(0,0,0,120), true, 1.0f);

    dl->AddText(ImVec2(p.x + 10, p.y + 8), IM_COL32(255,255,255,255), text);
}


static void DrawCommand(ImDrawList* dl, ImVec2 p, ImVec2 s, ImU32 color, const char* text, bool highlighted=false) {
    ImU32 body = color;
    ImU32 bodyDark = IM_COL32( (int)(((body)&0xFF))-20, (int)(((body>>8)&0xFF))-20, (int)(((body>>16)&0xFF))-20, 255);
    if (highlighted) {
        body = IM_COL32(255,255,255,200);
        bodyDark = IM_COL32(200,200,200,180);
    }
    dl->AddRectFilled(p, ImVec2(p.x+s.x, p.y+s.y), body, 6.0f);
    float notchW = 18.0f;
    dl->AddRectFilled(ImVec2(p.x - notchW*0.5f, p.y + 6.0f), ImVec2(p.x + notchW*0.6f, p.y + s.y - 6.0f), bodyDark, 6.0f);
    dl->AddRectFilled(ImVec2(p.x + s.x - 36, p.y + 8), ImVec2(p.x + s.x - 6, p.y + s.y - 8), bodyDark, 4.0f);
    dl->AddText(ImVec2(p.x + 12, p.y + 8), IM_COL32(255,255,255,255), text);
}

static void DrawChildArea(ImDrawList* dl, ImVec2 p, ImVec2 s) {
    dl->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), IM_COL32(40,40,45,255), 4.0f);
    dl->AddRect(p, ImVec2(p.x + s.x, p.y + s.y), IM_COL32(80,80,80,255), 4.0f);
}


struct ExecResult {
    bool ok;
    float number;
    bool boolean;
    ExecResult(): ok(false), number(0), boolean(false) {}
};

struct Executor {
    std::vector<Block*> queue;
    int index = 0;
    double nextTime = 0.0;
    bool running = false;
    double stepDuration = 0.35;

    ImVec2 actor = ImVec2(900, 200);
    float angle = 0.0f;

    void reset() { queue.clear(); index = 0; running = false; nextTime = 0.0; }

    ExecResult Evaluate(Block* b) {
        ExecResult r;
        if (!b) return r;
        if (b->kind == BlockKind::Reporter) {
            r.ok = true;
            std::string s(b->valueText);
            try {
                r.number = std::stof(s);
            } catch(...) {
                r.number = 0.0f;
            }
            return r;
        } else if (b->kind == BlockKind::Boolean) {
            r.ok = true;
            std::string s(b->valueText);
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            r.boolean = (s == "true" || s == "1");
            return r;
        }
        r.ok = false;
        return r;
    }

    void collect(const std::vector<Block*>& all) {
        queue.clear();
        for (Block* b : all) {
            bool isChild = false;
            for (Block* o : all) {
                for (auto &s : o->slots) {
                    if (s.kind == SlotKind::Statement && s.attached == b) { isChild = true; break; }
                }
                if (isChild) break;
            }
            if (!isChild && b->prev == nullptr && b->kind == BlockKind::Command) {
                Block* cur = b;
                while (cur) { queue.push_back(cur); cur = cur->next; }
            }
        }
    }

    void start(double now) {
        if (queue.empty()) return;
        running = true; index = 0; nextTime = now + stepDuration;
        if (!queue.empty()) queue[0]->highlighted = true;
    }

    void stop() {
        running = false;
        for (auto &b : queue) b->highlighted = false;
    }

    bool perform(Block* b) {
        if (!b) return false;
        // Example commands:
        if (b->label == "Move") {
            float amount = 40.0f;
            if (!b->slots.empty()) {
                void* att = b->slots[0].attached;
                if (att) {
                    Block* rep = (Block*)att;
                    ExecResult er = Evaluate(rep);
                    if (er.ok) amount = er.number;
                }
            }
            actor.x += amount;
            std::cout << "[Exec] Move " << amount << " -> actor.x=" << actor.x << "\n";
            return false;
        } else if (b->label == "Turn") {
            float deg = 30.0f;
            if (!b->slots.empty()) {
                void* att = b->slots[0].attached;
                if (att) {
                    Block* rep = (Block*)att;
                    ExecResult er = Evaluate(rep);
                    if (er.ok) deg = er.number;
                }
            }
            angle += deg;
            if (angle >= 360.0f) angle -= 360.0f;
            std::cout << "[Exec] Turn " << deg << " -> angle=" << angle << "\n";
            return false;
        } else if (b->label == "Repeat") {
            int times = 2;
            if (!b->slots.empty()) {
                void* att = b->slots[0].attached;
                if (att) { Block* rep = (Block*)att; ExecResult er = Evaluate(rep); if (er.ok) times = (int)er.number; }
            }
            if (b->slots.size() >= 2 && b->slots[1].attached) {
                Block* child = (Block*)b->slots[1].attached;
                for (int t=0;t<times;++t) {
                    Block* cur = child;
                    while (cur) {
                        cur->highlighted = true;
                        perform(cur);
                        cur->highlighted = false;
                        cur = cur->next;
                    }
                }
            }
            return false;
        } else if (b->label == "If") {
            bool cond = false;
            if (!b->slots.empty()) {
                void* att = b->slots[0].attached;
                if (att) { Block* bb = (Block*)att; ExecResult er = Evaluate(bb); if (er.ok) cond = er.boolean; }
            }
            if (cond && b->slots.size() >= 2 && b->slots[1].attached) {
                Block* child = (Block*)b->slots[1].attached;
                Block* cur = child;
                while (cur) { perform(cur); cur = cur->next; }
            }
            return false;
        }
        return false;
    }

    void update(double now) {
        if (!running) return;
        if (now < nextTime) return;

        // finish current
        if (index < (int)queue.size()) {
            Block* cur = queue[index];
            perform(cur);
            cur->highlighted = false;
        }
        index++;
        if (index >= (int)queue.size()) {
            running = false;
            return;
        }
        queue[index]->highlighted = true;
        nextTime = now + stepDuration;
    }
};


int main() {
    // init
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1280,720, "Atom Engine",nullptr,nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cerr<<"GLEW init failed\n"; return -1; }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");

    std::vector<Block*> blocks;

    auto paletteAdd = [&](BlockKind kind, const char* label, BlockKind createKind, float px, float py, std::vector<Slot> slotTemplates) {
        Block* p = new Block(kind,label,px,py);
        p->slots = slotTemplates;
        blocks.push_back(p);
    };

    Executor exec;

    // Seed with a couple of reporter/boolean blocks already in editor
    Block* num10 = new Block(BlockKind::Reporter, "10", 600, 80);
    std::strncpy(num10->valueText, "10", sizeof(num10->valueText));
    blocks.push_back(num10);

    Block* boolTrue = new Block(BlockKind::Boolean, "true", 600, 130);
    std::strncpy(boolTrue->valueText, "true", sizeof(boolTrue->valueText));
    blocks.push_back(boolTrue);

    // Utility to spawn new editor block at location
    auto spawnBlockAt = [&](const char* id, float localx, float localy)->Block* {
        // id may be "Move", "Turn", "Repeat", "Number", "BooleanTrue", "BooleanFalse", "If"
        if (strcmp(id,"Move")==0) {
            Block* b = new Block(BlockKind::Command,"Move", localx, localy);
            b->slots.push_back(Slot(SlotKind::Value, ImVec2(120,6), ImVec2(60,24)));
            blocks.push_back(b);
            return b;
        } else if (strcmp(id,"Turn")==0) {
            Block* b = new Block(BlockKind::Command,"Turn", localx, localy);
            b->slots.push_back(Slot(SlotKind::Value, ImVec2(100,6), ImVec2(60,24)));
            blocks.push_back(b);
            return b;
        } else if (strcmp(id,"Repeat")==0) {
            Block* b = new Block(BlockKind::Command,"Repeat", localx, localy);
            b->size = ImVec2(220,60);
            b->slots.push_back(Slot(SlotKind::Value, ImVec2(84,8), ImVec2(50,24)));
            b->slots.push_back(Slot(SlotKind::Statement, ImVec2(10,34), ImVec2(200,18)));
            blocks.push_back(b);
            return b;
        } else if (strcmp(id,"If")==0) {
            Block* b = new Block(BlockKind::Command,"If", localx, localy);
            b->size = ImVec2(220,60);
            b->slots.push_back(Slot(SlotKind::Boolean, ImVec2(60,8), ImVec2(48,24)));
            b->slots.push_back(Slot(SlotKind::Statement, ImVec2(10,34), ImVec2(200,18)));
            blocks.push_back(b);
            return b;
        } else if (strcmp(id,"Number")==0) {
            Block* b = new Block(BlockKind::Reporter, "0", localx, localy);
            std::strncpy(b->valueText,"0", sizeof(b->valueText));
            blocks.push_back(b);
            return b;
        } else if (strcmp(id,"True")==0) {
            Block* b = new Block(BlockKind::Boolean, "true", localx, localy);
            std::strncpy(b->valueText,"true", sizeof(b->valueText));
            blocks.push_back(b);
            return b;
        } else if (strcmp(id,"False")==0) {
            Block* b = new Block(BlockKind::Boolean, "false", localx, localy);
            std::strncpy(b->valueText,"false", sizeof(b->valueText));
            blocks.push_back(b);
            return b;
        }
        return nullptr;
    };

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        double now = glfwGetTime();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ---------------- Left: Palette ----------------
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(300,720));
        ImGui::Begin("Blocks", nullptr, ImGuiWindowFlags_NoCollapse);

        if (ImGui::CollapsingHeader("Motion")) {
            ImGui::TextWrapped("Commands:");
            if (ImGui::Selectable("Move")) {
                // spawn a Move at default palette area (or we could drag)
                spawnBlockAt("Move", 320, 80);
            }
            if (ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload("BLOCK_TYPE","Move",5);
                ImGui::TextUnformatted("Move");
                ImGui::EndDragDropSource();
            }

            if (ImGui::Selectable("Turn")) {
                spawnBlockAt("Turn", 320, 130);
            }
            if (ImGui::BeginDragDropSource()) { ImGui::SetDragDropPayload("BLOCK_TYPE","Turn",5); ImGui::TextUnformatted("Turn"); ImGui::EndDragDropSource(); }
        }

        if (ImGui::CollapsingHeader("Looks")) {
            if (ImGui::Selectable("Turn (alias)")) { spawnBlockAt("Turn", 320, 180); }
            if (ImGui::BeginDragDropSource()) { ImGui::SetDragDropPayload("BLOCK_TYPE","Turn",5); ImGui::TextUnformatted("Turn"); ImGui::EndDragDropSource(); }
        }

        if (ImGui::CollapsingHeader("Control")) {
            if (ImGui::Selectable("Repeat")) { spawnBlockAt("Repeat", 320, 220); }
            if (ImGui::BeginDragDropSource()) { ImGui::SetDragDropPayload("BLOCK_TYPE","Repeat",7); ImGui::TextUnformatted("Repeat"); ImGui::EndDragDropSource(); }

            if (ImGui::Selectable("If")) { spawnBlockAt("If", 320, 260); }
            if (ImGui::BeginDragDropSource()) { ImGui::SetDragDropPayload("BLOCK_TYPE","If",3); ImGui::TextUnformatted("If"); ImGui::EndDragDropSource(); }
        }

        if (ImGui::CollapsingHeader("Operators / Reporters")) {
            if (ImGui::Selectable("Number")) { spawnBlockAt("Number", 320, 300); }
            if (ImGui::BeginDragDropSource()) { ImGui::SetDragDropPayload("BLOCK_TYPE","Number",7); ImGui::TextUnformatted("Number"); ImGui::EndDragDropSource(); }

            if (ImGui::Selectable("True")) { spawnBlockAt("True", 320, 340); }
            if (ImGui::BeginDragDropSource()) { ImGui::SetDragDropPayload("BLOCK_TYPE","True",5); ImGui::TextUnformatted("True"); ImGui::EndDragDropSource(); }

            if (ImGui::Selectable("False")) { spawnBlockAt("False", 320, 380); }
            if (ImGui::BeginDragDropSource()) { ImGui::SetDragDropPayload("BLOCK_TYPE","False",6); ImGui::TextUnformatted("False"); ImGui::EndDragDropSource(); }
        }

        ImGui::End();

        // ---------------- Middle: Editor ----------------
        ImGui::SetNextWindowPos(ImVec2(300,0));
        ImGui::SetNextWindowSize(ImVec2(700,520));
        ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse);

        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 origin = ImGui::GetWindowPos();
        ImVec2 mouse = ImGui::GetIO().MousePos;

        // accept palette drops
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BLOCK_TYPE")) {
                const char* id = (const char*)payload->Data;
                float lx = mouse.x - origin.x;
                float ly = mouse.y - origin.y;
                spawnBlockAt(id, lx, ly);
            }
            ImGui::EndDragDropTarget();
        }

        // editor bg
        ImVec2 editorTL = ImGui::GetCursorScreenPos();
        ImVec2 editorAvail = ImGui::GetContentRegionAvail();
        dl->AddRectFilled(editorTL, ImVec2(editorTL.x + editorAvail.x, editorTL.y + editorAvail.y), IM_COL32(25,25,30,255), 4.0f);

        // Run/Stop buttons
        ImGui::SameLine();
        if (ImGui::Button("Run")) { exec.collect(blocks); exec.start(now); }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) { exec.stop(); }

        // Draw & interact blocks (painter order: back to front -> draw commands first, reporters/booleans on top)
        // We'll draw commands first so reporters and booleans appear above.
        for (Block* b : blocks) {
            if (b->kind != BlockKind::Command) continue;
            ImVec2 screenP = ImVec2(origin.x + b->pos.x, origin.y + b->pos.y);
            // choose semantic color for command label
            ImU32 col = IM_COL32(100,150,255,255);
            if (b->label == "Move") col = IM_COL32(46,204,113,255);
            if (b->label == "Turn") col = IM_COL32(255,193,7,255);
            if (b->label == "Repeat" || b->label == "If") col = IM_COL32(155,89,182,255);
            DrawCommand(dl, screenP, b->size, col, b->label.c_str(), b->highlighted);

            // draw slots inside
            for (int i=0;i<(int)b->slots.size();++i) {
                Slot &s = b->slots[i];
                ImVec2 sp = ImVec2(screenP.x + s.offset.x, screenP.y + s.offset.y);
                ImVec2 ep = ImVec2(sp.x + s.size.x, sp.y + s.size.y);
                if (s.kind == SlotKind::Value) {
                    // draw rounded input rect
                    dl->AddRectFilled(sp, ep, IM_COL32(240,240,240,255), 6.0f);
                    dl->AddRect(sp, ep, IM_COL32(120,120,120,255), 6.0f);
                    // draw attached reporter text
                    if (s.attached) {
                        Block* rep = (Block*)s.attached;
                        dl->AddText(ImVec2(sp.x+6, sp.y+4), IM_COL32(20,20,20,255), rep->valueText);
                    } else {
                        dl->AddText(ImVec2(sp.x+6, sp.y+4), IM_COL32(100,100,100,255), "value");
                    }
                } else if (s.kind == SlotKind::Boolean) {
                    // draw diamond-ish small box
                    ImVec2 bp = sp;
                    DrawBoolean(dl, bp, s.size, s.attached ? ((Block*)s.attached)->valueText : "?", false);
                } else if (s.kind == SlotKind::Statement) {
                    // child area
                    DrawChildArea(dl, sp, s.size);
                }
            }
        }

        // Next draw reporters & booleans (so they appear above)
        for (Block* b : blocks) {
            if (b->kind == BlockKind::Command) continue;
            ImVec2 screenP = ImVec2(origin.x + b->pos.x, origin.y + b->pos.y);
            if (b->kind == BlockKind::Reporter) {
                DrawReporter(dl, screenP, b->size, b->valueText[0] ? b->valueText : "0", b->highlighted);
            } else if (b->kind == BlockKind::Boolean) {
                DrawBoolean(dl, screenP, b->size, b->valueText[0] ? b->valueText : "?", b->highlighted);
            }
        }

        // Interaction: dragging, clicking, slot editing & snapping
        for (Block* b : blocks) {
            ImVec2 screenP = ImVec2(origin.x + b->pos.x, origin.y + b->pos.y);
            ImVec2 s = b->size;
            ImVec2 mouseGlobal = ImGui::GetIO().MousePos;

            // reporter editing: clicking a reporter opens small inline text input
            if (b->kind == BlockKind::Reporter) {
                if (mouseInRect(mouseGlobal, screenP, s) && ImGui::IsMouseClicked(0)) {
                    // open popup input (we'll reuse a single popup name per block pointer)
                    char popupName[32]; snprintf(popupName, sizeof(popupName),"reporter_edit_%p", (void*)b);
                    ImGui::OpenPopup(popupName);
                }
                // popup
                char popupName[32]; snprintf(popupName, sizeof(popupName),"reporter_edit_%p", (void*)b);
                ImGui::SetNextWindowPos(screenP);
                if (ImGui::BeginPopup(popupName, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove)) {
                    ImGui::InputText("##val", b->valueText, sizeof(b->valueText), ImGuiInputTextFlags_EnterReturnsTrue);
                    if (ImGui::IsItemDeactivatedAfterEdit()) ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
            }

            // boolean editing same as reporter (toggle quickly)
            if (b->kind == BlockKind::Boolean) {
                if (mouseInRect(mouseGlobal, ImVec2(screenP.x-2,screenP.y-2), ImVec2(s.x+4,s.y+4)) && ImGui::IsMouseClicked(0)) {
                    // toggle value
                    std::string cur(b->valueText);
                    std::transform(cur.begin(),cur.end(),cur.begin(),::tolower);
                    if (cur=="true") std::strncpy(b->valueText,"false",sizeof(b->valueText));
                    else std::strncpy(b->valueText,"true",sizeof(b->valueText));
                }
            }

            // Start drag for any block
            bool overBody = mouseInRect(mouseGlobal, screenP, s);
            if (overBody && ImGui::IsMouseClicked(0) && !b->dragging) {
                b->dragging = true;
                b->dragOffset = ImVec2(mouseGlobal.x - b->pos.x - origin.x, mouseGlobal.y - b->pos.y - origin.y);

                // detach linear stack
                if (b->prev) { b->prev->next = nullptr; b->prev = nullptr; }

                // detach from statement slot owners if any
                for (Block* other : blocks) {
                    for (auto &sl : other->slots) {
                        if (sl.kind == SlotKind::Statement && sl.attached == b) sl.attached = nullptr;
                    }
                }
                // also detach if attached as input to some slot
                for (Block* other : blocks) {
                    for (auto &sl : other->slots) {
                        if ((sl.kind == SlotKind::Value || sl.kind == SlotKind::Boolean) && sl.attached == b) sl.attached = nullptr;
                    }
                }
            }

            // Drag movement
            if (b->dragging) {
                if (ImGui::IsMouseDown(0)) {
                    float nx = ImGui::GetIO().MousePos.x - origin.x - b->dragOffset.x;
                    float ny = ImGui::GetIO().MousePos.y - origin.y - b->dragOffset.y;
                    b->pos = ImVec2(nx, ny);
                    // move chained next blocks beneath it
                    Block* cur = b->next;
                    float offsetY = b->size.y + 6.0f;
                    while (cur) {
                        cur->pos = ImVec2(b->pos.x, b->pos.y + offsetY);
                        cur = cur->next;
                        offsetY += cur->size.y + 6.0f;
                    }
                } else {
                    // release: try snapping
                    b->dragging = false;
                    bool snapped = false;
                    // try linear stacking under other command blocks
                    for (Block* other : blocks) {
                        if (other == b) continue;
                        if (other->kind != BlockKind::Command) continue;
                        ImVec2 otherBottom = ImVec2(other->pos.x, other->pos.y + other->size.y + 6.0f);
                        ImVec2 bTop = b->pos;
                        if (dsq(otherBottom.x, otherBottom.y, bTop.x, bTop.y) < 14.0f*14.0f) {
                            // snap under
                            b->pos = ImVec2(other->pos.x, other->pos.y + other->size.y + 6.0f);
                            other->next = b;
                            b->prev = other;
                            snapped = true;
                            break;
                        }
                    }
                    if (!snapped) {
                        // try snapping into typed slots (value/boolean/statement)
                        for (Block* other : blocks) {
                            if (other == b) continue;
                            for (auto &sl : other->slots) {
                                // compute slot screen rect
                                ImVec2 slotTop = ImVec2(other->pos.x + sl.offset.x, other->pos.y + sl.offset.y);
                                ImVec2 slotBottom = ImVec2(slotTop.x + sl.size.x, slotTop.y + sl.size.y);
                                ImVec2 center = ImVec2(b->pos.x + b->size.x*0.5f, b->pos.y + b->size.y*0.5f);
                                if (center.x > slotTop.x && center.x < slotBottom.x && center.y > slotTop.y && center.y < slotBottom.y) {
                                    // type check
                                    if ((sl.kind == SlotKind::Value && b->kind == BlockKind::Reporter) ||
                                        (sl.kind == SlotKind::Boolean && b->kind == BlockKind::Boolean) ||
                                        (sl.kind == SlotKind::Statement && b->kind == BlockKind::Command))
                                    {
                                        // attach
                                        // for value/boolean attach store pointer in sl.attached
                                        sl.attached = (void*)b;
                                        // place block visually inside slot (with small inset)
                                        b->pos = ImVec2(other->pos.x + sl.offset.x + 4.0f, other->pos.y + sl.offset.y + 4.0f);
                                        // mark prev to prevent being detected as top-level
                                        b->prev = other;
                                        snapped = true;
                                        break;
                                    }
                                }
                            }
                            if (snapped) break;
                        }
                    }
                }
            }

        } // end interaction loop

        ImGui::End(); // Editor

        // executor update
        exec.update(now);

        // ---------------- Right: Stage ----------------
        ImGui::SetNextWindowPos(ImVec2(300,520));
        ImGui::SetNextWindowSize(ImVec2(700,200));
        ImGui::Begin("Stage");
        ImDrawList* sdl = ImGui::GetWindowDrawList();
        // draw actor
        sdl->AddCircleFilled(exec.actor, 18.0f, IM_COL32(200,100,200,255));
        float r = exec.angle * 3.1415926f/180.0f;
        sdl->AddLine(exec.actor, ImVec2(exec.actor.x + cosf(r)*18.0f, exec.actor.y + sinf(r)*18.0f), IM_COL32(255,255,255,200), 2.0f);
        ImGui::Text("Actor (%.1f, %.1f) angle %.1f", exec.actor.x, exec.actor.y, exec.angle);
        ImGui::End();

        // ---------------- Render ----------------
        ImGui::Render();
        int dw, dh; glfwGetFramebufferSize(window, &dw, &dh);
        glViewport(0,0,dw,dh);
        glClearColor(0.12f,0.12f,0.14f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
