import sys, os
from manim import *

"""
Manim Community v0.18.1

# 最简 —— 用启动器(封装了下面所有环境变量),先 `xlings install cairo pango uv` 一次:
#   videos/render.sh cpp11/00-auto-and-decltype.py        # -ql 预览;加 -qh 出高清
#
# 推荐 —— xlings 生态(无需系统 gcc):图形栈已发布到 xlings-res 并进官方索引
# (deps 自动拉全树)。手动等价命令如下,详见 videos/README.md。
#
#   xlings install cairo pango                # 装进项目匿名 subos _ (xlings-gcc 默认搜它)
#   SR=$PWD/.xlings/subos/_/usr               # 仓库根下的项目 subos, 不是全局 default
#   PKG_CONFIG_PATH=$SR/lib/pkgconfig \
#     uv run --with "manim==0.18.1" --with "pygments==2.17.2" \
#     manim -ql videos/cpp11/00-auto-and-decltype.py        # -ql 预览 / -qh 高清
#
# 兜底 —— 没装生态栈时,用系统 gcc 绕开 xlings-gcc 的 xcb 坑:
#   CC=/usr/bin/gcc CXX=/usr/bin/g++ uv run --with "manim==0.18.1" --with "pygments==2.17.2" manim -ql videos/cpp11/00-auto-and-decltype.py
"""

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from d2x import *
from d2x import DHighlight

class AutoAndDecltype(MovingCameraScene):
    def construct(self):

        title, logo = mcpp_video_start(self, "{ 类型自动推导 }")
        self.camera.frame.save_state()          # 记录初始机位, 后面括号陷阱推近后拉回

        self.wait(0.5)

        auto_text = Text("auto", color=BLUE).shift(UL * 2).scale(0.7)
        decltype_text = Text("decltype", color=GREEN).shift(DR * 2).scale(0.7)
        auto_demo_text = Text("auto func() -> int", color=PURPLE).shift(DL * 1.5).scale(0.5)
        decltype_demo_text = Text("decltype(obj.data)", color=ORANGE).shift(UR * 1.5).scale(0.5)

        self.play(
            LaggedStart(
                ReplacementTransform(title.copy(), auto_text),
                ReplacementTransform(title.copy(), decltype_text),
                ReplacementTransform(title.copy(), auto_demo_text),
                ReplacementTransform(title.copy(), decltype_demo_text),
                lag_ratio=0.25,
            )
        )

        self.wait(0.5)

        # 场景1: 声明定义 —— auto 从初始值推导, decltype 取声明类型
        self.play(
            FadeOut(auto_text),
            FadeOut(decltype_text),
            FadeOut(auto_demo_text),
            FadeOut(decltype_demo_text),
            Transform(title, Text("{ 1 - 声明定义 }", t2c = { '声明定义': BLUE }))
        )

        self.wait(0.3)

        # 显式类型声明
        explicit_code = self.create_code_helper(
            'int    a = 1;\n'
            'double b = 2.0;\n'
            'char   c = \'c\';'
        )

        # 右: auto 版
        auto_code = self.create_code_helper(
            'auto a = 1;\n'
            'auto b = 2.0;\n'
            'auto c = \'c\';'
        ).shift(RIGHT * 2.5)

        label_l = Text("显式类型", color=GRAY, font_size=26).next_to(explicit_code, UP, buff=0.25)
        label_r = Text("auto 推导", font_size=26).next_to(auto_code, UP, buff=0.25)

        # 先显明确的类型
        self.play(
            ReplacementTransform(title, explicit_code),
            FadeIn(label_l)
        )
        self.wait(0.5)

        # 高亮显式类型关键字
        self.play(
            DHighlight(explicit_code.code[0][0:3], color=YELLOW),
            DHighlight(explicit_code.code[1][0:6], color=YELLOW),
            DHighlight(explicit_code.code[2][0:4], color=YELLOW),
        )

        self.wait(0.5)

        # 显示 auto 版本
        explicit_code = VGroup(explicit_code, label_l)
        auto_code = VGroup(auto_code, label_r)
        self.play(
            ReplacementTransform(explicit_code.copy(), auto_code),
            Transform(explicit_code, explicit_code.copy().scale(0.8).shift(LEFT * 2.5))
        )

        self.wait(0.5)

        auto_and_decltype_code = self.create_code_helper(
            'auto a = 1;\n'
            'auto b = 2.0;\n'
            'decltype(a) a1 = 1;\n'
            'decltype(2.0) b1 = 2.0;'
        ).move_to(explicit_code)

        auto_and_decltype_code = VGroup(
            auto_and_decltype_code,
            Text("auto & decltype", color=BLUE, font_size=26).next_to(auto_and_decltype_code, UP, buff=0.25)
        )

        self.play(
            Transform(auto_code, auto_code.copy().scale(0.8)),
            ReplacementTransform(explicit_code, auto_and_decltype_code)
        )

        self.wait(0.5)

        auto_vs_type = self.create_code_helper(
            'std::string str      = { "hello" };\n'
            'std::vector<int> vec = { 1, 2, 3 };\n'
            'Rectangle rect       = Rectangle { 1, 2 };\n'
            '// --- \n'
            'auto str  = std::string { "hello" };\n'
            'auto vec  = std::vector<int> { 1, 2, 3 };\n'
            'auto rect = Rectangle { 1, 2 };'
        )

        self.play(ReplacementTransform(VGroup(
            auto_and_decltype_code,
            auto_code,
        ), auto_vs_type))

        self.wait(0.5)


        # 场景2: 表达式类型推导
        title = Text("{ 2 - 表达式类型推导 }", t2c={'表达式类型': BLUE})
        self.play(ReplacementTransform(auto_vs_type, title))

        self.wait(0.5)

        code_expr = self.create_code_helper(
            'int a = 1;\n'
            'int    b1 = a + 2 + 1.1;\n'
            'double b2 = a + 2 + 1.1;\n'
        )

        self.play(ReplacementTransform(title, code_expr))

        self.wait(0.5)

        self.play(
            DHighlight(code_expr.code[1][12:]),
            DHighlight(code_expr.code[2][12:])
        )

        self.wait(0.5)

        b1_value_text = Text("4").next_to(code_expr, 2.5 * UL, buff=0.25).scale(0.5)
        b2_value_text = Text("4.1").next_to(code_expr, 2.5 * DL, buff=0.25).scale(0.5)
        b1_arrow_to_text = Arrow(
            code_expr.code[1][7].get_left(),
            b1_value_text.get_bottom(),
            color=YELLOW,
            stroke_width=2,
            tip_length=0.1,
            buff=0.05,
        ).set_opacity(0.6)

        b2_arrow_to_text = Arrow(
            code_expr.code[2][7].get_left(),
            b2_value_text.get_top(),
            color=YELLOW,
            stroke_width=2,
            tip_length=0.12,
            buff=0.1,
        ).set_opacity(0.6)

        expr_type_double_text = Text("表达式类型double").to_edge(0.2 * RIGHT).scale(0.4).set_color(PURE_RED)
        self.play(Write(expr_type_double_text))
        self.wait(0.5)

        self.play(
            FadeIn(b1_value_text),
            FadeIn(b2_value_text),
            Create(b1_arrow_to_text),
            Create(b2_arrow_to_text),
        )

        self.wait(0.5)

        auto_and_decltype_code = self.create_code_helper(
            'auto x = a + 2 + 1.1;\n'
            'decltype(a + 2 + 1.1) y = x;'
        )

        self.play(ReplacementTransform(VGroup(
            code_expr,
            b1_value_text, b2_value_text,
            b1_arrow_to_text, b2_arrow_to_text,
            expr_type_double_text,
        ), auto_and_decltype_code))
        self.wait(0.5)

        assert_type_code = self.create_code_helper(
            'auto c = \'0\' + 1;\n'
            'static_assert((std::is_same_v<decltype(c), ? >));'
        )
        self.play(ReplacementTransform(auto_and_decltype_code, assert_type_code))
        self.wait(0.5)
        self.play(DHighlight(assert_type_code.code[1][35:36], color=PURE_RED, scale_factor=2))


        # 场景3: 复杂类型推导
        title = Text("{ 3 - 复杂类型推导 }", t2c={'复杂类型': BLUE})
        self.play(ReplacementTransform(assert_type_code, title))
        self.wait(0.5)

        code_iterator = self.create_code_helper("""
std::vector<int> v = {1, 2, 3};

// std::vector<int>::iterator it = v.begin();
auto it = v.begin(); // 自动推导it类型
                                                
// decltype(v.begin()) it = v.begin();
for (; it != v.end(); ++it) {
    if (*it == 2) {
        v.insert(it, 0);
        break;
    }
}
""")
        code_iterator = VGroup(code_iterator, Text("STL 迭代器类型冗长难写").next_to(code_iterator, UP, buff=0.25).scale(0.5))
        code_iterator[0].code[2].set_color(GREEN)
        code_iterator[0].code[5].set_color(GREEN)

        self.play(ReplacementTransform(title, code_iterator))
        self.wait(0.5)

        self.play(DHighlight(code_iterator[0].code[2][3:30]))
        self.wait(0.5)
        self.play(DHighlight(code_iterator[0].code[3][0:5]))
        self.wait(0.5)
        self.play(DHighlight(code_iterator[0].code[5][3:23]))
        self.wait(0.5)

        code_func_type = self.create_code_helper("""
int add_func(int a, int b) {
    return a + b;
}

int main() {
    auto minus_func = [](int a, int b) { return a - b; };

    std::vector<std::function<decltype(add_func)>> funcVec = {
        add_func,
        minus_func
    };

    funcVec[0](1, 2);
    funcVec[1](1, 2);
}
""")
        code_func_type = VGroup(code_func_type, Text("函数类型和Lambda").next_to(code_func_type, UP, buff=0.25).scale(0.5))
        code_func_type.scale(0.85)

        self.play(ReplacementTransform(code_iterator, code_func_type))
        self.wait(0.5)
        self.play(DHighlight(code_func_type[0].code[5][22:]))
        self.play(DHighlight(code_func_type[0].code[5][4:9]))
        self.wait(0.5)
        self.play(DHighlight(code_func_type[0].code[7][30:48]))
        self.wait(0.5)

        code_func_type[0].code[7][30:48].set_color(PURE_RED)
        code_func_type[0].code[5][4:9].set_color(PURE_RED)
        self.play(
            code_func_type[0].code[7][30:48].animate.set_color(PURE_RED),
            code_func_type[0].code[5][4:9].animate.set_color(PURE_RED)
        )
        self.wait(0.5)
        self.play(DHighlight(code_func_type[0].code[7][30:48], color=PURE_RED, scale_factor=1.5))
        self.wait(0.5)
        self.play(DHighlight(code_func_type[0].code[5][4:9], color=PURE_RED, scale_factor=1.5))


        # 场景4: 类体成员类型推导
        title = Text("{ 4 - 类成员类型推导 }", t2c={'类成员类型': BLUE})
        self.play(ReplacementTransform(code_func_type, title))
        self.wait(0.5)

        code_class_member = self.create_code_helper("""
struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    const Object obj;

    auto a = obj.a; // auto -> int
    std::vector<decltype(obj.b)> vec;
    // vector<decltype(obj.b)> -> vector<double>
}""")
        code_class_member = VGroup(code_class_member, Text("类成员类型推导").next_to(code_class_member, UP, buff=0.25).scale(0.5))

        self.play(ReplacementTransform(title, code_class_member))
        self.wait(0.5)

        self.play(DHighlight(code_class_member[0].code[9][20:]))
        code_class_member[0].code[9][20:].set_color(YELLOW)
        #self.play(DHighlight(code_class_member[0].code[11][4:]))
        #code_class_member[0].code[11][4:].set_color(YELLOW)

        self.wait(0.5)


        # 场景5: 标准库STL示例
        title = Text("{ 5 - 标准库STL示例 }", t2c={'标准库STL': BLUE})
        self.play(ReplacementTransform(code_class_member, title))
        self.wait(0.5)

        code_filesystem_iterator_1 = self.create_code_helper("""
// MSVC STL · msvc-stl/stl/inc/filesystem
auto _New_end = _Vec.begin();
for (auto _Pos = _Vec.begin(); _Pos != _Vec.end();) {
    const auto _Elem = *_Pos++;
    // ...（根据 _Elem 决定是否写回 _New_end, 略）
}
""")
        code_filesystem_iterator_1.code[0].set_color(GREEN)
        code_filesystem_iterator_1 = VGroup(code_filesystem_iterator_1, Text("auto 推导迭代器与元素类型").next_to(code_filesystem_iterator_1, UP, buff=0.25).scale(0.5))
        self.play(ReplacementTransform(title, code_filesystem_iterator_1))
        self.wait(0.5)

        code_lower_bound = self.create_code_helper("""
// MSVC STL · msvc-stl/stl/inc/xutility — std::lower_bound
auto _UFirst = _STD _Get_unwrapped(_First);
auto _Count  = _STD distance(_UFirst, _STD _Get_unwrapped(_Last));

while (0 < _Count) { // divide and conquer, find half that contains answer
    const auto _Count2 = static_cast<decltype(_Count)>(_Count / 2);
    const auto _UMid   = _STD next(_UFirst, _Count2);
    // ...（在 _UMid 处比较, 缩小区间, 略）
}
""")
        code_lower_bound.code[1].set_color(GREEN)

        code_lower_bound = VGroup(code_lower_bound, Text("decltype 取变量的类型").next_to(code_lower_bound, UP, buff=0.25).scale(0.5))
        
        code_lower_bound.scale(0.8)

        self.play(ReplacementTransform(code_filesystem_iterator_1, code_lower_bound))
        self.wait(0.5)

        code_lower_bound[0].code[5][37:53].set_color(PURE_RED)
        self.play(DHighlight(code_lower_bound[0].code[5][37:53], color=PURE_RED))
        self.wait(0.5)

        # 场景6: 注意事项
        title = Text("{ 6 - 注意事项 }", t2c={'注意事项': BLUE})
        self.play(ReplacementTransform(code_lower_bound, title))
        self.wait(0.5)

        code_auto_const_ref = self.create_code_helper("""
int a = 1;
int &b = a;
const int c = 1;
const int &d = c;

auto a1 = a; // int
auto b1 = b; // int
auto c1 = c; // int
auto d1 = d; // int
""")
        self.play(ReplacementTransform(title, code_auto_const_ref))
        self.wait(0.5)

        for line_id in range(5, 9):
            code_auto_const_ref.code[line_id][13:].set_color(PURE_RED)
            self.play(DHighlight(code_auto_const_ref.code[line_id][13:], color=PURE_RED), run_time=0.25)
        self.wait(0.5)

        code_save_const_ref = self.create_code_helper("""
const auto c2 = c;  // const int
const auto &d2 = d; // const int &

decltype(c) c3 = c; // const int
decltype(d) d3 = d; // const int &
""")
        code_save_const_ref.shift(RIGHT * 2.5).scale(0.8)
        code_auto_const_ref_copy = code_auto_const_ref.copy()
        self.play(
            ReplacementTransform(code_auto_const_ref_copy, code_save_const_ref),
            code_auto_const_ref.animate.shift(LEFT * 3.5).scale(0.6)
        )
        self.wait(0.5)

        for line_id in range(0, 5):
            code_save_const_ref.code[line_id][21:].set_color(GREEN)
            self.play(DHighlight(code_save_const_ref.code[line_id][21:], color=GREEN), run_time=0.25)

        self.wait(0.5)

        code_decltype_note1 = self.create_code_helper("""
int a = 1;
decltype(a) b;         // 推导结果为a的声明类型int 
decltype( (a) ) c = a; // 推导结果为(a)这个左值表达式的类型 int &
""")
        code_decltype_note1.scale(0.8)
        self.play(ReplacementTransform(
            VGroup(code_save_const_ref, code_auto_const_ref),
            code_decltype_note1
        ))
        
        for line_id in range(1, 3):
            code_decltype_note1.code[line_id][22:].set_color(PURE_RED)
            self.play(DHighlight(code_decltype_note1.code[line_id][22:], color=PURE_RED), run_time=0.25)

        self.wait(0.5)

        code_decltype_note2 = self.create_code_helper("""
struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    Object obj;
    const Object obj1;

    decltype(obj.b)     // double
    decltype(obj1.b)    // double

    decltype( (obj.b) ) // double &

    // 受obj1定义的const修饰影响, 所以是 const double &
    decltype( (obj1.b) )
}
""")
        code_decltype_note3 = self.create_code_helper("""
int &&b = 1;

decltype(b)     // 推导结果是声明类型 int &&
decltype( (b) ) // 推导结果是 int &
""")
        
        code_decltype_note2.code[15].set_color(PURE_RED)
        code_decltype_note3.code[2][16:].set_color(PURE_RED)
        code_decltype_note3.code[3][16:].set_color(PURE_RED)
        code_decltype_note2.scale(0.8)

        self.play(ReplacementTransform(code_decltype_note1, code_decltype_note2))
        self.wait(0.5)
        self.play(ReplacementTransform(code_decltype_note2, code_decltype_note3))
        self.wait(0.5)

        mcpp_video_end(self, logo, VGroup(code_decltype_note2, code_decltype_note3))

    @staticmethod
    def create_code_helper(code: str):
        return Code(
            code=code,
            background="",
            language="cpp",
        )

if __name__ == "__main__":
    scene = AutoAndDecltype()
    scene.render()
