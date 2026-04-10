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

    def translate(points, dx, dy):
        res = []
        for x, y in points:
            res.append((x + dx, y + dy))
        return res
    
    def scale_around_pivot(points, pivot, sx, sy):
        px, py = pivot
        res = []
        for x, y in points:
            new_x = px + (x - px) * sx
            new_y = py + (y - py) * sy
            res.append((new_x, new_y))
        return res


    class Task_10(Scene):

        def __init__(self, mode, **kwargs):
            super().__init__(**kwargs)

            pivot = (0.5, 0.5)
            dx, dy = 1, -1
            sx = 2
            sy = 2

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
            angle = np.radians(30)

            if mode == 1:
                # Масштабування -> Обертання -> Зсув
                step1 = scale_around_pivot(points, pivot, sx, sy)
                step2 = rotate_around_pivot(step1, pivot, angle)
                new_coords = translate(step2, dx, dy)

            elif mode == 2:
                # Зсув -> Масштабування -> Обертання
                step1 = scale_around_pivot(points, pivot, sx, sy)
                step2 = rotate_around_pivot(step1, pivot, angle)
                new_coords = translate(step2, dx, dy)

            elif mode == 3:
                # Масштабування -> Зсув -> Обертання
                step1 = translate(points, dx, dy)
                step2 = scale_around_pivot(step1, pivot, sx, sy)
                new_coords = rotate_around_pivot(step2, pivot, angle)

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
        pivot["vertex_color"] = "orange"
        pivot["vertex_size"] = 30


    scene_1 = Task_10(
        mode=1,
        image_size=(6, 6),
        coordinate_rect=(-2, -3, 4, 4),
        title="Завдання 10: Масштабування -> Обертання -> Зсув Pivot (0.5, 0.5)",
        grid_show=True,
        axis_show=True
    )

    scene_2 = Task_10(
        mode=2,
        image_size=(6, 6),
        coordinate_rect=(-2, -3, 4, 3),
        title="Завдання 10: Зсув -> Масштабування -> Обертання Pivot (0.5, 0.5)",
        grid_show=True,
        axis_show=True
    )

    scene_3 = Task_10(
        mode=3,
        image_size=(6, 6),
        coordinate_rect=(-2, -3, 5, 3),
        title="Завдання 10: Масштабування -> Зсув -> Обертання Pivot (0.5, 0.5)",
        grid_show=True,
        axis_show=True
    )

    scene_1.add_frames(setup_scene)
    scene_1.show()

    scene_2.add_frames(setup_scene)
    scene_2.show()

    scene_3.add_frames(setup_scene)
    scene_3.show()