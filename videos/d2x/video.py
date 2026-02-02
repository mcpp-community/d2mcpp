from manim import *

def mcpp_video_start(scene, target_title):

    title = Text(target_title)
    logo = VGroup(
        Tex(r"\textit{\underline{d2mcpp}}"),
        Tex(r"\textit{+}", color=RED)
    ).arrange(RIGHT, buff=0.1).scale(0.8)

    scene.play(Write(logo), run_time=0.8)
    scene.bring_to_front(logo)

    scene.play(
        logo.animate.to_corner(UP + LEFT),
        FadeIn(title),
    )

    return title, logo

def mcpp_video_end(scene, logo, obj_group=VGroup()):

    scene.play(
        logo.animate.move_to(ORIGIN),
        FadeOut(obj_group),
        run_time=0.7
    )

    scene.wait(0.5)

    ending = VGroup(
        Text("开源交互式教程", color=RED).scale(0.9),
        Tex(r"\textit{\underline{https://github.com/mcpp-community/d2mcpp}}"),
    ).arrange(DOWN, buff=0.15).scale(0.8)

    scene.play(
        ReplacementTransform(logo[1], ending[0]),
        ReplacementTransform(logo[0], ending[1]),
    )

    scene.play(FadeOut(ending), run_time = 3)