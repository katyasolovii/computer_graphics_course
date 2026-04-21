import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.SimplePolygon import SimplePolygon
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
    TRI_START_KEY = "tri_start"
    TRI_ANIMATED_KEY = "tri_animated"
    PIVOT_KEY = "pivot_point"

    v1, v2, v3 = (1, 2, 3), (4, 5, 6), (7, 8, 9)

    class TriangleScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[TRI_START_KEY] = SimplePolygon(self.plt_axis, v1, v2, v3, 
                                                color="blue", alpha=0.3)
            self[TRI_ANIMATED_KEY] = SimplePolygon(self.plt_axis, v1, v2, v3, 
                                                   color="grey", alpha=0.5, line_style="-.")

            self[PIVOT_KEY] = PointModel(self.plt_axis, (2.0, 3.0, 4.0), color="fuchsia", size=50)

    animated_scene = TriangleScene(
        coordinate_rect=(-2, -2, -2, 10, 10, 12),
        title="Завдання 8: Поворот навколо осі (1,1,1), що не проходить через початок координат \n pivot (2,3,4) та зсув. \n Оскільки три точки лежать на одній прямій, \n то такий 'трикутник' вироджується у відрізок."
    )

    pivot_p = (2, 3, 4)
    axis_v = (1, 1, 1)
    angle = 90
    final_move = (0, -3, 2)

    T_to_origin = Mat4x4.translation(-pivot_p[0], -pivot_p[1], -pivot_p[2])
    T_back = Mat4x4.translation(pivot_p[0], pivot_p[1], pivot_p[2])
    vx, vy, vz = axis_v
    phi = math.degrees(math.atan2(vx, vz))
    v_z_p = math.sqrt(vx**2 + vz**2)
    theta = math.degrees(math.atan2(vy, v_z_p))

    Ry = Mat4x4.rotation_y(-phi, is_radians=False)
    Rx = Mat4x4.rotation_x(theta, is_radians=False)
    Rz = Mat4x4.rotation_z(angle, is_radians=False)

    R_axis = Ry.transpose() * Rx.transpose() * Rz * Rx * Ry
    M_pivot_rot = T_back * R_axis * T_to_origin
    T_final = Mat4x4.translation(*final_move)
    M_total = T_final * M_pivot_rot


    animation = TrsTransformationAnimation(
        end=M_total,            
        channel=TRI_ANIMATED_KEY,
        frames=120,             
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )
    animated_scene.add_animations(animation)
    animated_scene.show()