import sys, os
from manim import *

"""
Manim Community v0.18.1

# 用 uv 运行(无需预装 manim):
uv run --with "manim==0.18.1" manim -pql videos/cpp11/00-auto-and-decltype.py
uv run --with "manim==0.18.1" manim -pqh videos/cpp11/00-auto-and-decltype.py
"""

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from d2x import *

class AutoAndDecltype(MovingCameraScene):
    def construct(self):

        title, logo = mcpp_video_start(self, "{ auto / decltype }")

        self.wait(0.5)

        self.play(Transform(title, Text("{ 类型自动推导 }", t2c={'类型自动推导': BLUE})))

        self.wait(0.5)

        # 场景1: 声明定义 —— auto 从初始值推导, decltype 取声明类型
        self.play(Transform(title, Text("{ 1 - 声明定义 }", t2c={'声明定义': BLUE})))

        self.wait(0.5)

        code1 = self.create_code_helper("""int b = 2;
auto b1 = b;          // b1: int
decltype(b) b2 = b;   // b2: int""")

        self.play(ReplacementTransform(title, code1))

        self.wait(0.5)

        self.play(DHighlight(code1.code[1]))            # auto: 从初始值推导

        self.wait(0.5)

        self.play(DHighlight(code1.code[2]))            # decltype: 取声明类型

        self.wait(0.5)

        # 场景2: 表达式类型推导 —— 保留计算精度
        title = Text("{ 2 - 表达式类型推导 }", t2c={'表达式': BLUE})
        self.play(ReplacementTransform(code1, title))

        self.wait(0.5)

        code2 = self.create_code_helper("""int a = 1;
auto x = a + 2 + 1.1;            // double
decltype(a + 2 + 1.1) y = x;     // double""")

        self.play(ReplacementTransform(title, code2))

        self.wait(0.5)

        self.play(DHighlight(code2.code[1], color=GREEN))
        self.play(DHighlight(code2.code[2], color=GREEN))

        self.wait(0.5)

        # 场景3: 复杂类型推导 —— auto 驯服冗长的迭代器类型(STL 真实场景)
        title = Text("{ 3 - 复杂类型推导 }", t2c={'复杂类型': BLUE})
        self.play(ReplacementTransform(code2, title))

        self.wait(0.5)

        code3a = self.create_code_helper("""std::vector<int> v = {1, 2, 3};
std::vector<int>::iterator it = v.begin();""")

        self.play(ReplacementTransform(title, code3a))

        self.wait(0.5)

        self.play(DHighlight(code3a.code[1], color=PURE_RED))   # 又长又难写

        self.wait(0.5)

        code3b = self.create_code_helper("""std::vector<int> v = {1, 2, 3};
auto it = v.begin();""")

        self.play(ReplacementTransform(code3a, code3b))

        self.wait(0.5)

        self.play(DHighlight(code3b.code[1], color=GREEN))      # auto 一步到位

        self.wait(0.5)

        # 场景4: 函数返回值类型推导 —— 后置返回 + decltype
        title = Text("{ 4 - 函数返回值推导 }", t2c={'函数返回值': BLUE})
        self.play(ReplacementTransform(code3b, title))

        self.wait(0.5)

        code4 = self.create_code_helper("""template<class T1, class T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}""")

        self.play(ReplacementTransform(title, code4))

        self.wait(0.5)

        self.play(DHighlight(code4.code[1]))            # auto ... -> decltype(a + b)

        self.wait(0.5)

        # 场景5: 注意事项 —— auto 剥离 const / 引用, 想保留要显式写
        title = Text("{ 5 - const / 引用剥离 }", t2c={'const / 引用': PURE_RED})
        self.play(ReplacementTransform(code4, title))

        self.wait(0.5)

        code5 = self.create_code_helper("""const int ci = 1;
auto a = ci;          // int   (const lost!)
const auto& r = ci;   // const int&
decltype(ci) d = ci;  // const int""")

        self.play(ReplacementTransform(title, code5))

        self.wait(0.5)

        self.play(DHighlight(code5.code[1], color=PURE_RED))    # 顶层 const 被剥离

        self.wait(0.5)

        self.play(DHighlight(code5.code[2], color=GREEN))       # const auto& 保留
        self.play(DHighlight(code5.code[3], color=GREEN))       # decltype 精确保留

        self.wait(0.5)

        # 场景6: 注意事项 —— decltype 的括号陷阱
        title = Text("{ 6 - decltype 括号陷阱 }", t2c={'括号陷阱': PURE_RED})
        self.play(ReplacementTransform(code5, title))

        self.wait(0.5)

        code6 = self.create_code_helper("""int a = 1;
decltype(a)   b;   // int
decltype((a)) c;   // int&""")

        self.play(ReplacementTransform(title, code6))

        self.wait(0.5)

        self.play(DHighlight(code6.code[1]))            # decltype(a): 声明类型 int

        self.wait(0.5)

        select_box = SurroundingRectangle(code6.code[2], color=PURE_RED, buff=0.1)
        self.play(Create(select_box))
        self.play(DHighlight(code6.code[2], color=PURE_RED))    # decltype((a)): 左值表达式 -> int&

        self.wait(0.5)

        mcpp_video_end(self, logo, VGroup(code6, select_box))

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
