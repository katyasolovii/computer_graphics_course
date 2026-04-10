import numpy as np
from src.engine.model.Polygon import Polygon
from src.engine.scene.Scene import Scene

if __name__ == '__main__':

    ORIGINAL_KEY = "square"
    TRANSFORMED_KEY = "square_transformed"
    PIVOT_KEY = "pivot"


    def rotate_around_pivot(points, pivot, angle):
        px, py = pivot
        cos_a = np.cos(angle)
        sin_a = np.sin(angle)
        res = []
        for x, y in points:
            new_x = px + (x - px) * cos_a - (y - py) * sin_a
            new_y = py + (x - px) * sin_a + (y - py) * cos_a
            res.append((new_x, new_y))
        return res


    class Task_07(Scene):

        def __init__(self, pivot, **kwargs):
            super().__init__(**kwargs)

            self.pivot = pivot

            square = Polygon()
            self[ORIGINAL_KEY] = square
            square.set_geometry(
                0, 0,
                1, 0,
                1, 1,
                0, 1
            )

            trans_square = Polygon()
            self[TRANSFORMED_KEY] = trans_square

            points = [(0,0),(1,0),(1,1),(0,1)]
            angle = np.radians(60)

            new_coords = rotate_around_pivot(points, pivot, angle)
            trans_square.set_geometry(
                new_coords[0][0], new_coords[0][1],
                new_coords[1][0], new_coords[1][1],
                new_coords[2][0], new_coords[2][1],
                new_coords[3][0], new_coords[3][1],
            )

            pivot_obj = Polygon()
            self[PIVOT_KEY] = pivot_obj
            px, py = pivot
            pivot_obj.set_geometry(px, py) 


    def setup_scene(scene):
        square = scene[ORIGINAL_KEY]
        square["color"] = "blue"
        square["vertices_show"] = True
        square["vertex_color"] = "black"

        trans = scene[TRANSFORMED_KEY]
        trans["color"] = "red"
        trans["vertices_show"] = True
        trans["vertex_color"] = "black"

        pivot = scene[PIVOT_KEY]
        pivot["color"] = "orange"
        pivot["vertices_show"] = True
        pivot["vertex_color"] = "fuchsia"
        pivot["vertex_size"] = 30


    scene_1 = Task_07(
        pivot=(0.5, 0.5),
        image_size=(6, 6),
        coordinate_rect=(-2, -2, 3, 3),
        title="Завдання 7: Поворот навколо опорної точки Pivot (0.5, 0.5)",
        grid_show=True,
        axis_show=True
    )

    scene_2 = Task_07(
        pivot=(0, 1),
        image_size=(6, 6),
        coordinate_rect=(-2, -2, 3, 3),
        title="Завдання 7: Поворот навколо опорної точки Pivot (0, 1)",
        grid_show=True,
        axis_show=True
    )

    scene_3 = Task_07(
        pivot=(1, 1),
        image_size=(6, 6),
        coordinate_rect=(-2, -2, 3, 3),
        title="Завдання 7: Поворот навколо опорної точки Pivot (1, 1)",
        grid_show=True,
        axis_show=True
    )

    scene_4 = Task_07(
        pivot=(2, 2),
        image_size=(6, 6),
        coordinate_rect=(-2, -2, 5, 5),
        title="Завдання 7: Поворот навколо опорної точки Pivot (2, 2)",
        grid_show=True,
        axis_show=True
    )


    scene_1.add_frames(setup_scene)
    scene_1.show()

    scene_2.add_frames(setup_scene)
    scene_2.show()

    scene_3.add_frames(setup_scene)
    scene_3.show()

    scene_4.add_frames(setup_scene)
    scene_4.show()