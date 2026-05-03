import numpy as np
import math

from src.engine.animation.QuaternionAnimation import QuaternionAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Quaternion import Quaternion

CUBE_KEY = "cube"

class AnimScene(AnimatedScene):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        cube = Cube(self.plt_axis, alpha=0.1, color='lightgreen')
        self[CUBE_KEY] = cube
        cube.show_pivot()
        cube.show_local_frame()

if __name__ == '__main__':
    animated_scene = AnimScene(
        title="Завдання 1: Операція повороту вектора (Z, 90°)",
        image_size=(10, 10),
        coordinate_rect=(-1, -1, -1, 1, 1, 1),
    )

    theta = math.radians(90)
    q_rot = Quaternion.rotation_z(theta)
    animation_quat = QuaternionAnimation(
        end_quaternion=q_rot,
        channel=CUBE_KEY
    )

    animated_scene.add_animations(
        animation_quat,
    )

    animated_scene.show()