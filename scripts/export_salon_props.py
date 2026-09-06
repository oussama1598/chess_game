"""Export the CC0 Poly Haven salon props to the runtime's OBJ convention.

Run with Blender 4.x:
  blender --background --factory-startup --python scripts/export_salon_props.py -- \
    /path/to/WoodenChair_01.gltf /path/to/fancy_picture_frame_01.gltf assets/models/salon

The exporter evaluates imported geometry, shares one origin across each asset,
triangulates on export, preserves authored UVs/normals, and converts Blender's
Z-up coordinates to the renderer's Y-up convention.
"""

from pathlib import Path
import sys

import bpy


def arguments() -> tuple[Path, Path, Path]:
    values = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []
    if len(values) != 3:
        raise RuntimeError("Expected chair glTF, frame glTF, and output directory")
    output = Path(values[2]).resolve()
    output.mkdir(parents=True, exist_ok=True)
    return Path(values[0]).resolve(), Path(values[1]).resolve(), output


def clear_scene() -> None:
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.object.delete(use_global=False)


def import_meshes(path: Path) -> list[bpy.types.Object]:
    clear_scene()
    bpy.ops.import_scene.gltf(filepath=str(path))
    return [item for item in bpy.context.scene.objects if item.type == "MESH"]


def normalize_asset(meshes: list[bpy.types.Object]) -> None:
    world_positions = [
        item.matrix_world @ vertex.co
        for item in meshes
        for vertex in item.data.vertices
    ]
    center_x = (min(point.x for point in world_positions) +
                max(point.x for point in world_positions)) * 0.5
    center_y = (min(point.y for point in world_positions) +
                max(point.y for point in world_positions)) * 0.5
    bottom_z = min(point.z for point in world_positions)

    for item in meshes:
        transform = item.matrix_world.copy()
        for vertex in item.data.vertices:
            point = transform @ vertex.co
            vertex.co = (point.x - center_x, point.y - center_y, point.z - bottom_z)
        item.matrix_world.identity()
        item.data.update()


def export_object(item: bpy.types.Object, output: Path) -> None:
    bpy.ops.object.select_all(action="DESELECT")
    item.select_set(True)
    bpy.context.view_layer.objects.active = item
    bpy.ops.wm.obj_export(
        filepath=str(output),
        check_existing=False,
        export_selected_objects=True,
        apply_modifiers=True,
        export_uv=True,
        export_normals=True,
        export_colors=False,
        export_materials=False,
        export_triangulated_mesh=True,
        forward_axis="NEGATIVE_Z",
        up_axis="Y",
    )
    print(f"Exported {item.name} -> {output}")


chair_source, frame_source, destination = arguments()

chair_meshes = import_meshes(chair_source)
normalize_asset(chair_meshes)
export_object(next(item for item in chair_meshes if item.name == "WoodenChair_01"),
              destination / "wooden_chair.obj")

frame_meshes = import_meshes(frame_source)
normalize_asset(frame_meshes)
export_object(next(item for item in frame_meshes
                   if item.name == "fancy_picture_frame_01"),
              destination / "picture_frame.obj")
export_object(next(item for item in frame_meshes
                   if item.name == "fancy_picture_frame_01_canvas"),
              destination / "picture_canvas.obj")
