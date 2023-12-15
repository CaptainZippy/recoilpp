#include "pch.h"

// Generic

// TODO add template<typename T>
struct Atom {
    Atom(const char* id, int initial) : _initial(initial) {}
    int _initial{};
};

// State contains rendered UI from state.
struct State;

// Application specific.

// Application Model contains keys for accessing application state
namespace Model {
const Atom width{"width", 100};
const Atom height{"height", 200};
}  // namespace Model


// Application View contains rendered UI from state.
namespace View {
struct Text;
struct Button;
struct List;
struct Frame;

using Element = std::variant<Frame, Text, List, Button>;

struct Text {
    std::string s;
};
struct Frame {
    std::function<View::Element(State&)> render;
};
struct List {
    std::vector<Element> e;
};
struct Button {
    std::string text;
    std::function<void()> onClick;
};

void print(const Element& e) {
    struct Visitor {
        std::vector<char> _indent;
        Visitor() { _indent.push_back(0); }
        void operator()(const Text& t) { printf("%sT %s\n", _indent.data(), t.s.c_str()); }
        void operator()(const Button& t) { printf("%sB\n", _indent.data()); }
        void operator()(const Frame& t) { printf("%sF\n", _indent.data()); }
        void operator()(const List& l) {
            printf("%sL (%zu)\n", _indent.data(), l.e.size());
            _indent.back() = '\t';
            _indent.push_back(0);
            for (auto&& e : l.e) {
                std::visit(*this, e);
            }
            _indent.pop_back();
            _indent.back() = 0;
        }
    } vis;
    std::visit(vis, e);
}
}  // namespace Data

using RenderFunc = std::function<View::Element(State&)>;

struct State {
    using Setter = std::function<void(int)>;
    std::map<const Atom*, int> _map;
    std::tuple<int, Setter> access(const Atom& a) {
        auto [it, inserted] = _map.emplace(&a, a._initial);
        return {it->second, [it](int x) { it->second = x; }};
    }
    int read(const Atom& a) {
        auto [it, inserted] = _map.emplace(&a, a._initial);
        return it->second;
    }
    View::Element frame(RenderFunc r) { return r(*this); }
};

View::Element renderChild(State& s) {
    return View::Text{"OK"};
}

View::Element renderTop(State& s) {
    auto [w, setw] = s.access(Model::width);
    auto h = s.read(Model::width);

    return View::List{{View::Text{std::format("Height value={}", h)},
                       View::Text{std::format("Width value={}", w)}, s.frame(renderChild),
                       View::Button{.text = "Add..", .onClick = []() {}}}};
}

struct Root {
    State _state;
    RenderFunc _render;
    std::string _id;
    Root(const char* id, RenderFunc r) : _id(id), _render(std::move(r)) {}

    std::map<Atom*, std::vector<Atom*>> _memo;

    void _push(const char* id) {}
    void _pop() {}

    void iter() {
        std::vector<int> stk;
        //_push(_stk, _id);
        auto s = _render(_state);
        //_pop();
        View::print(s);
        std::string action;
        std::cin >> action;
        if (action == "+") {
            /*auto [w, setw] = width.access();
            setw(w + 1);*/
        }
    }
};

int main() {
    Root s{"root", renderTop};
    while (1) {
        s.iter();
    }
    return 0;
}