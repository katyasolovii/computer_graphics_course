import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Quaternion import Quaternion

CUBE_KEY = "decomposed_cube"

class DecompScene(AnimatedScene):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        cube = Cube(self.plt_axis, alpha=0.3, color='lightgreen')
        self[CUBE_KEY] = cube
        cube.show_pivot()
        cube.show_local_frame()

if __name__ == '__main__':
    animated_scene = DecompScene(
        title="Завдання 5: Етапи декомпозиції (S -> R -> T), створення даної матриці M",
        coordinate_rect=(-5, -5, -3, 12, 12, 12), 
    )

    T_vec = (10, -5, 3)
    S_vec = (1, 2, 1.5)
    q_rot = Quaternion.rotation_z(math.radians(90))

    M_scale = Mat4x4.scale(*S_vec)
    M_scale_rot = q_rot.toRotationMatrix() * M_scale
    M_total = Mat4x4.translation(*T_vec) * M_scale_rot

    anim_1_scale = TrsTransformationAnimation(
        end=M_scale, channel=CUBE_KEY, frames=60, repeat=False
    )
    anim_2_rotation = TrsTransformationAnimation(
        end=M_scale_rot, channel=CUBE_KEY, frames=60, repeat=False
    )
    anim_3_translation = TrsTransformationAnimation(
        end=M_total, channel=CUBE_KEY, frames=60, repeat=False
    )

    animated_scene.add_animations(
        anim_1_scale,
        anim_2_rotation,
        anim_3_translation
    )

    animated_scene.show()