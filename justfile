# compiler
cc := "gcc"
# compiler flags
cflags := "-Wall -Wextra -ggdb"

# executable directory
bin_dir := "bin"
# library directory
lib_dir := bin_dir
# include directory
inc_dir := "include -I include/freetype2"
# source directory
src_dir := "src"
# shader directory
shader_dir := "shaders"
# resources directory
resources_dir := "resources"


# source files
src := src_dir / "main.c " + src_dir / "util.c " + src_dir / "shader.c " + src_dir / "renderer.c " + src_dir / "lineal.c " + src_dir / "glyphs.c " + src_dir / "button.c " + src_dir / "glad.c"
# executable path
bin := bin_dir / "cterm"
# libraries
libs := if os_family() == "windows" { "-lopengl32 -lglfw3 -lfreetype" } else { "-lGL -lglfw -lm -lfreetype" }


build:
    mkdir -p {{bin_dir}}
    {{cc}} {{cflags}} -I {{inc_dir}} {{src}} -o {{bin}} -L {{lib_dir}} {{libs}}
    cp -r {{shader_dir}} {{bin_dir}}
    cp -r {{resources_dir}} {{bin_dir}}

run:
    ./{{bin}}

clean:
    rm -rf {{bin}} {{bin_dir}}/{{shader_dir}}/ {{bin_dir}}/{{resources_dir}}