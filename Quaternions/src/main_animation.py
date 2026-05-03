import numpy as np

from src.engine.animation.QuaternionAnimation import QuaternionAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Quaternion import Quaternion

CUBE_KEY = "cube"

angle_x, angle_y, angle_z = np.radians(
    (30, 45, 60)
)

qx = Quaternion.rotation_x(angle_x)
qy = Quaternion.rotation_y(angle_y)
qz = Quaternion.rotation_z(angle_z)
q_final = qz * qy * qx

animation_quat = QuaternionAnimation(
    end_quaternion=q_final,
    channel=CUBE_KEY,
)


class AnimScene(AnimatedScene):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        cube = Cube(self.plt_axis, alpha=0.1)
        self[CUBE_KEY] = cube
        cube.show_pivot()
        cube.show_local_frame()


if __name__ == '__main__':
    animated_scene = AnimScene(
        title="",  # заголовок рисунка
        image_size=(10, 10),  # розмір зображення: 1 - 100 пікселів
        coordinate_rect=(-1, -1, -1, 1, 1, 1),  # розмірність системи координат
    )

    animated_scene.add_animations(
        animation_quat,
    )

    animated_scene.show()
