clang++ -fsanitize=address -g -O3 -Wno-c++11-compat-deprecated-writable-strings -Wno-deprecated-declarations -Wno-parentheses -Wno-writable-strings -std=c++0x adm-fudge.cpp -o adm-info `pkg-config --cflags --libs glib-2.0` -lm
