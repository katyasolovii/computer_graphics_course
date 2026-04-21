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

    class CubeScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[CUBE_START_KEY] = Cube(self.plt_axis, alpha=0.1, color="blue")
            self[CUBE_ANIMATED_KEY] = Cube(self.plt_axis, alpha=0.1, color="grey", line_style="-.")
            self[PIVOT_KEY] = PointModel(self.plt_axis, (1.0, 1.0, 1.0), color="fuchsia", size=50)

    animated_scene = CubeScene(
        coordinate_rect=(-4, -1, -1, 5, 6, 6),
        title="Завдання 10: Комплексна трансформація (Scale X(x2), Rot Y(45°) \n навколо (1,1,1) та Trans (-3,4,2))"
    )

    pivot = (1, 1, 1)
    final_move = (-3, 4, 2)
    
    T_to_origin = Mat4x4.translation(-pivot[0], -pivot[1], -pivot[2])
    T_back = Mat4x4.translation(pivot[0], pivot[1], pivot[2])
    Sx = Mat4x4.scale(2, 1, 1)
    M_scale_pivot = T_back * Sx * T_to_origin
    Ry = Mat4x4.rotation_y(45, is_radians=False)
    M_rot_pivot = T_back * Ry * T_to_origin
    T_final = Mat4x4.translation(*final_move)
    M_total = T_final * M_rot_pivot * M_scale_pivot

    animation_move = TrsTransformationAnimation(
        end=M_total,            
        channel=CUBE_ANIMATED_KEY,
        frames=150,             
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )

    animated_scene.add_animations(animation_move)
    animated_scene.show()