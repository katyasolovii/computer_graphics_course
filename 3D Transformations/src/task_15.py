import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Vec4 import Vec4
from src.engine.model.Model import Model
from src.base.points import draw_point


class PointModel(Model):
    def __init__(self, plt_axis, coords, color="fuchsia", size=50):
        super().__init__(plt_axis, *coords) 
        self.point_color = color
        self.point_size = size

    def draw_model(self):
        p = self.transformed_geometry[0]
        draw_point(self.plt_axis, p.xyz, color=self.point_color, size=self.point_size)

if __name__ == '__main__':
    CUBE_START_KEY = "cube_start"
    CUBE_ANIMATED_KEY = "cube_animated"
    PIVOT_KEY = "pivot_point"

    class FinalComplexScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[CUBE_START_KEY] = Cube(self.plt_axis, alpha=0.05, color="blue")
            self[CUBE_ANIMATED_KEY] = Cube(self.plt_axis, alpha=0.15, color="grey", line_style="-.")
            self[PIVOT_KEY] = PointModel(self.plt_axis, (1.0, 1.0, 1.0), color="fuchsia", size=50)

    animated_scene = FinalComplexScene(
        coordinate_rect=(-5, -2, -2, 5, 7, 7),
        title="Завдання 15: Складна композиція з опорною точкою та декомпозиція"
    )

    pivot = (1, 1, 1)
    final_move = (-3, 4, 2)
    T_to_origin = Mat4x4.translation(-pivot[0], -pivot[1], -pivot[2])
    T_back = Mat4x4.translation(pivot[0], pivot[1], pivot[2])
    S = Mat4x4.scale(2, 2, 2)
    M1_pivot_scale = T_back * S * T_to_origin
    Rx_local = Mat4x4.rotation_x(90, is_radians=False)
    M2_local_rot = M1_pivot_scale * Rx_local
    T_global = Mat4x4.translation(*final_move)
    M_total = T_global * M2_local_rot

    animation = TrsTransformationAnimation(
        end=M_total,            
        channel=CUBE_ANIMATED_KEY,
        frames=180,             
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )

    animated_scene.add_animations(animation)
    animated_scene.show()