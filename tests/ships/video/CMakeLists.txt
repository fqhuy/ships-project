include_directories("/usr/include/ImageMagick/")
set(deps ships_core ships_io ships_video Magick++)
make_test(test_ships_video ${deps})
