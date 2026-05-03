import numpy as np

from src.engine.model.SimplePolygon import SimplePolygon
from src.engine.model.Vector import Vector
from src.engine.scene.Scene import Scene
from src.math.Mat4x4 import Mat4x4

if __name__ == '__main__':
    RECT_KEY = "rect"
    VECT_KEY = "vector"


    class SimplePolygonScene(Scene):

        def __init__(self, **kwargs):
            super().__init__(**kwargs)

            polygon = SimplePolygon(self.plt_axis,
                                    1, 1, 1,
                                    1 + 0.557, 1 + 0.500, 1 + 0.663,
                                    1, 1 + 1, 1,
                                    edgecolor="red",
                                    )
            self[RECT_KEY] = polygon

            vector = Vector(self.plt_axis,
                            1, 1, 1,
                            1 + 0.557, 1 + 0.500, 1 + 0.663
                            )
            self[VECT_KEY] = vector
            vector.color = "brown"


    ############## Frame 1 ##################
    def frame1(scene: Scene):
        rect: SimplePolygon = scene[RECT_KEY]

        # rect.color = "blue"  # колір ліній
        rect.alpha = 0.2


    ############## Frame 2 ##################
    def frame2(scene: Scene):
        rect: SimplePolygon = scene[RECT_KEY]
        vect: Vector = scene[VECT_KEY]

        Tp = Mat4x4.translation(vect[0])
        Tp_1 = Tp.inverse()

        rect.transformation = Tp_1
        vect.transformation = Tp_1


    ############## Frame 3 ##################
    def frame3(scene: Scene):
        rect: SimplePolygon = scene[RECT_KEY]
        rect.color = "blue"
        vect: Vector = scene[VECT_KEY]

        O = vect[0]
        v1 = vect[1]
        v = v1 - O

        Tp = Mat4x4.translation(O)
        Tp_1 = Tp.inverse()

        phy = np.arctan2(v.x, v.z)
        R_phy_oy = Mat4x4.rotation_y(-phy)

        R = R_phy_oy * Tp_1

        rect.transformation = R
        vect.transformation = R


    ############## Frame 4 ##################
    def frame4(scene: Scene):
        rect: SimplePolygon = scene[RECT_KEY]
        rect.color = "blue"
        vect: Vector = scene[VECT_KEY]

        O = vect[0]
        v1 = vect[1]
        v = v1 - O

        Tp = Mat4x4.translation(O)
        Tp_1 = Tp.inverse()

        phy = np.arctan2(v.x, v.z)
        R_phy_oy = Mat4x4.rotation_y(-phy)

        print(v.xz)
        theta = np.arctan2(v.y, np.linalg.norm(v.xz))
        print(np.degrees(theta))
        R_theta_ox = Mat4x4.rotation_x(theta)

        R = R_theta_ox * R_phy_oy * Tp_1

        rect.transformation = R
        vect.transformation = R


    ############## Frame 5 ##################
    def frame5(scene: Scene):
        rect: SimplePolygon = scene[RECT_KEY]
        rect.color = "red"
        vect: Vector = scene[VECT_KEY]

        O = vect[0]
        v1 = vect[1]
        v = v1 - O

        Tp = Mat4x4.translation(O.xyz)
        Tp_1 = Tp.inverse()

        phy = np.arctan2(v.x, v.z)
        R_phy_oy = Mat4x4.rotation_y(-phy)

        print(v.xz)
        theta = np.arctan2(v.y, np.linalg.norm(v.xz))
        print(np.degrees(theta))
        R_theta_ox = Mat4x4.rotation_x(theta)

        R_psy_oz = Mat4x4.rotation_z(np.radians(90))

        R = R_psy_oz * R_theta_ox * R_phy_oy * Tp_1

        rect.transformation = R
        vect.transformation = R


    ############## Frame 6 ##################
    def frame6(scene: Scene):
        rect: SimplePolygon = scene[RECT_KEY]
        rect.color = "red"
        vect: Vector = scene[VECT_KEY]

        O = vect[0]
        v1 = vect[1]
        v = v1 - O

        Tp = Mat4x4.translation(O)
        Tp_1 = Tp.inverse()

        phy = np.arctan2(v.x, v.z)
        R_phy_oy = Mat4x4.rotation_y(-phy)

        print(v.xz)
        theta = np.arctan2(v.y, np.linalg.norm(v.xz))
        print(np.degrees(theta))
        R_theta_ox = Mat4x4.rotation_x(theta)

        R_psy_oz = Mat4x4.rotation_z(np.radians(90))

        R = Tp_1.inverse() * R_phy_oy.inverse() * R_theta_ox.inverse() * R_psy_oz * R_theta_ox * R_phy_oy * Tp_1

        rect.transformation = R
        vect.transformation = R


    simple_scene = SimplePolygonScene(
        image_size=(5, 5),  # розмір зображення: 1 - 100 пікселів
        coordinate_rect=(-1, -1, -1, 2, 2, 2),  # розмірність системи координатps
        title="Picture",  # заголовок рисунка
        grid_show=False,  # чи показувати координатну сітку
        base_axis_show=False,  # чи показувати базові осі зображення
        axis_show=True,  # чи показувати осі координат
        # axis_color="grey",  # колір осей координат
        axis_line_style="-."  # стиль ліній осей координат
    )

    simple_scene.add_frames(
        frame1,
        frame2,
        frame3,
        frame4,
        frame5,
        frame6,
    )  # додаємо кадри на сцену

    simple_scene.show()
