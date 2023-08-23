
#clang++ -fsanitize=address -O3 -Wno-c++11-compat-deprecated-writable-strings -Wno-deprecated-declarations -Wno-parentheses -Wno-writable-strings -std=c++14 ./tests/test_adm-fudge.cpp -o test-adm `pkg-config --cflags --libs glib-2.0` -lgtest -lgtest_main -pthread -lm -L/Users/Black/Documents/GitHub/taas-fudge/lib/gtest

#clang++ -std=c++14 /Users/Black/Documents/GitHub/taas-fudge/tests/test_adm-fudge.cpp -o test-adm -L/Users/Black/Documents/GitHub/taas-fudge/lib/gtest -lgtest -lgtest_main -pthread
#./test-adm
clang++ -fsanitize=address -O3 -Wno-c++11-compat-deprecated-writable-strings -Wno-deprecated-declarations -Wno-parentheses -Wno-writable-strings -std=c++0x adm-fudge.cpp -o adm-fudge `pkg-config --cflags --libs glib-2.0` -lm
