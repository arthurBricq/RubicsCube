#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

/**
 * The colors of the rubicscube
 */
enum Color {
    WHITE, RED, YELLOW, ORANGE, GREEN, BLUE, NONE 
};

using namespace glm;
using std::cout;
using std::endl;

/**
 * Holds the position and the orientation of 1 cube
 */
class Cube {
    private:
        void set_values(vec3 _pos, float _angle, vec3 _axis) {
            position = _pos;
            orientation = angleAxis(radians(_angle), _axis);
        }

    public:
        glm::vec3 position;
        glm::quat orientation;
        Color color1 = Color::NONE;
        Color color2 = Color::NONE;
        Color color3 = Color::NONE;

        Cube(vec3 _pos, Color _c1) {
            set_values(_pos, 0., vec3());
            color1 = _c1;
        }

        Cube(vec3 _pos, Color _c1, Color _c2) {
            // C++ is really a fucked up language. 
            // I trully don't know why, but you can't call the other constructor.
            // If you do then it will reset some values
            set_values(_pos, 0., vec3());
            color1 = _c1;
            color2 = _c2;
        }

        Cube(vec3 _pos, Color _c1, Color _c2, Color _c3) {
            set_values(_pos, 0., vec3());
            color1 = _c1;
            color2 = _c2;
            color3 = _c3;
        }
        
        /**
         * Swap around the x-axis
         */
        void swap_x() {
            orientation = angleAxis(radians(180.0f), vec3(0., 0., 1.)) * orientation;
        }

        /**
         * Rotate around the x-axis with a given angle in degrees
         */
        void rotate_x(float _theta) {
            orientation = angleAxis(radians(_theta), vec3(1., 0., 0.)) * orientation;
        }

        /**
         * Rotate around the y-axis with a given angle in degrees
         */
        void rotate_y(float _theta) {
            orientation = angleAxis(radians(_theta), vec3(0., 1., 0.)) * orientation;
        }

        /**
         * Rotate around the z-axis with a given angle in degrees
         */
        void rotate_z(float _theta) {
            orientation = angleAxis(radians(_theta), vec3(0., 0., 1.)) * orientation;
        }

        /**
         * Fills inplace the array with the colors of the Front, Right & Top colors
         */
        void fillColors(Color* _toFill) const {
            _toFill[0] = color1;
            _toFill[1] = color2;
            _toFill[2] = color3;
        }

        void debug() const {
            cout << "Colors = " << color1 << "," << color2 << "," << color3 << endl;
            cout << "Pos    = " << position.x << "," << position.y << endl;
        }
};


/**
 * The model for a rubicscube is simply a list of cubes...
 */
class RubicsCube {
    public:
        std::vector<Cube> cubes;

        RubicsCube() {

            // 0. CENTERS

            // White
            cubes.push_back(Cube(vec3(1., 1., 2.), Color::WHITE));

            // Orange
            cubes.push_back(Cube(vec3(1., 2., 1.), Color::ORANGE));
            cubes.back().rotate_x(-90.0f);

            // Red
            cubes.push_back(Cube(vec3(1., 0., 1.), Color::RED));
            cubes.back().rotate_x(90.0f);

            // Blue
            cubes.push_back(Cube(vec3(2., 1., 1.), Color::BLUE));
            cubes.back().rotate_y(90.0f);

            // Green
            cubes.push_back(Cube(vec3(0., 1., 1.), Color::GREEN));
            cubes.back().rotate_y(-90.0f);


            // 1. WHITE FACE

            // Left = green
            // Right = Blue
            // Top = Orange
            // Bottom = Red


            // Left 
            cubes.push_back(Cube(vec3(0., 1., 2.), Color::WHITE, Color::GREEN));
            cubes.back().swap_x();

            // Right
            cubes.push_back(Cube(vec3(2., 1., 2.), Color::WHITE, Color::BLUE));

            // Top
            cubes.push_back(Cube(vec3(1., 2., 2.), Color::WHITE, Color::ORANGE));
            cubes.back().rotate_z(90.0f);

            // Bottom
            cubes.push_back(Cube(vec3(1., 0., 2.), Color::WHITE, Color::RED));
            cubes.back().rotate_z(-90.0f);

            // Top-Right
            cubes.push_back(Cube(vec3(2., 2., 2.), Color::WHITE, Color::BLUE, Color::ORANGE));

            // Top-left
            cubes.push_back(Cube(vec3(0., 2., 2.), Color::WHITE, Color::GREEN, Color::ORANGE));
            cubes.back().swap_x();

            // Bottom Right
            cubes.push_back(Cube(vec3(2., 0., 2.), Color::WHITE, Color::BLUE, Color::RED));

            // Bottom Left
            cubes.push_back(Cube(vec3(0., 0., 2.), Color::WHITE, Color::GREEN, Color::RED));
            cubes.back().swap_x();


            
            // 2. ORANGE FACE 
            // bottom is already done


            // Right
            cubes.push_back(Cube(vec3(2., 2., 1.), Color::ORANGE, Color::BLUE));
            cubes.back().rotate_x(-90.0f);

            // Left
            cubes.push_back(Cube(vec3(0., 2., 1.), Color::ORANGE, Color::GREEN));
            cubes.back().swap_x();
            cubes.back().rotate_x(-90.0f);



            // cubes.push_back(Cube(vec3(1., 1., 0.), 0., vec3(), Color::WHITE, Color::YELLOW));
            // cubes.push_back(Cube(vec3(2., 1., 0.), 0., vec3(), Color::WHITE, Color::YELLOW));
            cout << "size = " << cubes.size() << endl;
            cout << "test = " << Color::NONE << endl;
        }
};
