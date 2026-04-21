import numpy as np
import math
import random

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Model import Model
from src.engine.model.SimplePolygon import SimplePolygon
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Vec4 import Vec4


class Tetrahedron(Model):
    def __init__(self, plt_axis, color="grey", alpha=1.0):
        super().__init__(plt_axis)
        self.polygons = []
        v = [
            [0, 0, 0], 
            [1, 0, 0],
            [0, 1, 0], 
            [0, 0, 1] 
        ]

        faces_indices = [
            [0, 1, 2],
            [0, 1, 3],
            [0, 2, 3], 
            [1, 2, 3]  
        ]

        for idx in faces_indices:
            face_vertices = [v[i] for i in idx]
            self.polygons.append(
                SimplePolygon(self.plt_axis, 
                              *face_vertices, 
                              color=color, 
                              alpha=alpha)
            )

    def draw_model(self):
        for polygon in self.polygons:
            polygon.transformation = self.transformation
            polygon.pivot(self._pivot)
            polygon.draw()

    def apply_transformation_to_geometry(self):
        super().apply_transformation_to_geometry()
        for polygon in self.polygons:
            polygon.apply_transformation_to_geometry()


if __name__ == '__main__':
    TETRA_START_KEY = "tetra_start"
    TETRA_ANIMATED_KEY = "tetra_animated"

    rand_angle = random.uniform(10, 90)
    rx, ry, rz = random.uniform(-1, 1), random.uniform(-1, 1), random.uniform(-1, 1)
    rand_axis = Vec4(rx, ry, rz)
    tx, ty, tz = random.uniform(-5, 5), random.uniform(-5, 5), random.uniform(-5, 5)
    rand_t = (tx, ty, tz)

    class TetraScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[TETRA_START_KEY] = Tetrahedron(self.plt_axis, color="blue", alpha=0.1)
            self[TETRA_ANIMATED_KEY] = Tetrahedron(self.plt_axis, color="grey", alpha=0.3)

    title_text = (f"Завдання 5: Кут повороту={round(rand_angle)},\n"
                  f"Вісь обертання=({round(rx,1)}, {round(ry,1)}, {round(rz,1)}),\n"
                  f"Вектор переміщення=({round(tx,1)}, {round(ty,1)}, {round(tz,1)})")

    animated_scene = TetraScene(
        coordinate_rect=(-3, -3, -3, 6, 6, 6),
        title=title_text
    )

    phi = math.degrees(math.atan2(rand_axis.x, rand_axis.z))
    v_z_prime = math.sqrt(rand_axis.x**2 + rand_axis.z**2)
    theta = math.degrees(math.atan2(rand_axis.y, v_z_prime))

    Ry = Mat4x4.rotation_y(-phi, is_radians=False)
    Rx = Mat4x4.rotation_x(theta, is_radians=False)
    Rz = Mat4x4.rotation_z(rand_angle, is_radians=False)

    R_final = Ry.transpose() * Rx.transpose() * Rz * Rx * Ry
    T_final = Mat4x4.translation(*rand_t)
    M_total = T_final * R_final

    animation = TrsTransformationAnimation(
        end=M_total,
        channel=TETRA_ANIMATED_KEY,
        frames=150,
        repeat=False
    )
    animated_scene.add_animations(animation)
    animated_scene.show()