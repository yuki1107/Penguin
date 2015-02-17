/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1

    Instructions:
        See main.cpp for more instructions.

        This file contains the OpenGL portion of the main window.
**************************************************************************/

#ifndef __GLWidget_h__
#define __GLWidget_h__

#include "include_gl.h"
#include "GLState.h"
#include "GLShape.h"
#include <QtOpenGL/QtOpenGL>

// Before transformed, this class displays a unit square centered at the
// origin.
class UnitSquare : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
	// Use two triangles to create the square.
        GLfloat square_vertices[][2] =
        {
            { -0.5, -0.5 },  // Triangle 1
            {  0.5, -0.5 },
            {  0.5,  0.5 },
            { -0.5, -0.5 },  // Triangle 2
            {  0.5,  0.5 },
            { -0.5,  0.5 },
        };

        initialize(
	    shader_input_location,
            reinterpret_cast<const GLfloat *>(square_vertices),
            /*num_vertices=*/6,
	    GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

class Mouth : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
    // Use two triangles to create the square.
        GLfloat square_vertices[][2] =
        {
            { -0.5, -0.1 },  // Triangle 1
            {  0.5, -0.1 },
            {  0.5,  0.25 },
            { -0.5, -0.1 },  // Triangle 2
            {  0.5,  0.25 },
            { -0.5,  0.1 },
        };

        initialize(
        shader_input_location,
            reinterpret_cast<const GLfloat *>(square_vertices),
            /*num_vertices=*/6,
        GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

class Arm : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
    // Use two triangles to create the square.
        GLfloat square_vertices[][2] =
        {
            { -0.3, -0.5 },  // Triangle 1
            {  0.3, -0.5 },
            {  0.5,  0.5 },
            { -0.3, -0.5 },  // Triangle 2
            {  0.5,  0.5 },
            { -0.5,  0.5 },
        };

        initialize(
        shader_input_location,
            reinterpret_cast<const GLfloat *>(square_vertices),
            /*num_vertices=*/6,
        GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

class Body : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
    // Use 4 triangles to create the square.
        GLfloat square_vertices[][2] =
        {
            { -0.6, 1.5 }, 
            {  -1.5,  -1 },
            { -0.5,  -1.5 },

            { -0.6, 1.5 }, 
            {  0.6, 1.5 },
            { -0.5,  -1.5 },

            { 0.6, 1.5 },
            { -0.5,  -1.5 },
            {  0.5,  -1.5 },

            {  0.5,  -1.5 },
            {  0.6, 1.5 },
            {  1.5, -1 }, 
        };

        initialize(
        shader_input_location,
            reinterpret_cast<const GLfloat *>(square_vertices),
            /*num_vertices=*/12,
        GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

class Head : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
    // Use 3 triangles to create the square.
        GLfloat square_vertices[][2] =
        {
            { -0.6, -0.9 }, 
            { -0.4, 0.9 },
            { -0.1, 1.1 },

            { -0.6, -0.9 }, 
            { -0.1, 1.1  },
            { 0.6, -0.9 },

            { -0.1, 1.1 },
            { 0.6, -0.9 },
            { 0.4, 0.9 },
        };

        initialize(
        shader_input_location,
            reinterpret_cast<const GLfloat *>(square_vertices),
            /*num_vertices=*/9,
        GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

// Before transformed, this class displays a unit circle centered at the
// origin.
class UnitCircle : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location, int num_circle_segments)
    {
        // We will draw a circle as a triangle fan.  We are careful to send
	// the second coordinate twice to properly close the circle.
        //        3     2     1
        //         +----+----+
        //        / \   |   /
        //       /   \  |  /
        //      /     \ | /
        //     /       \|/
        //   4+---------+ 0
        //        ...
        std::vector<GLfloat> circle_vertices;
        circle_vertices.push_back(0.0);
        circle_vertices.push_back(0.0);
        for (int i=0; i<=num_circle_segments; ++i)
        {
            double angle = (2 * M_PI * i) / num_circle_segments;
            circle_vertices.push_back(cos(angle));
            circle_vertices.push_back(sin(angle));
        }

        initialize(
	    shader_input_location,
            &circle_vertices[0],
            num_circle_segments + 2,
	    GL_TRIANGLE_FAN);
    }
};

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    // These values control the bounds to display on the joint angle sliders.
    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Add different ranges for the different joints.  Use these ranges
    //   when constructing sliders and when animating joints.
    //////////////////////////////////////////////////////////////////////////
    static const int HEAD_MIN = -45 * 0.2;
    static const int HEAD_MAX = 45 * 0.2;

    static const int LLEG_MIN = -45;
    static const int LLEG_MAX = 45;

    static const int LFEET_MIN = -45;
    static const int LFEET_MAX = 45;

    static const int RLEG_MIN = -45;
    static const int RLEG_MAX = 45;

    static const int RFEET_MIN = -45;
    static const int RFEET_MAX = 45;

    static const int ARM_MIN = -45;
    static const int ARM_MAX = 45;

    static const int MOUTH_MIN = -45 * 0.1;
    static const int MOUTH_MAX = 45 * 0.1;

    GLWidget(QWidget *parent=NULL);

public slots:
    // This method is called when the user changes the joint angle slider.
    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   There is currently only one joint, but you need to add more.
    //////////////////////////////////////////////////////////////////////////
    void setArmAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        arm_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setHeadAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        head_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setLeftLegAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        left_leg_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setLeftFeetAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        left_feet_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setRightLegAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        right_leg_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setRightFeetAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        right_feet_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setMouthAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        mouth_angle = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setHorizontalAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        xmove = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void setVerticalAngle(int angle)
    {
        // This method is called when the user changes the slider value.
        ymove = angle;

        // Call update() to trigger a redraw.
        update();
    }

    void onPressAnimate(int is_animating)
    {
        // This method is called when the user changes the animation checkbox.
        m_is_animating = (bool)is_animating;
        m_animation_frame = 0;
        stop = 0;
        update();
    }

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void timerEvent(QTimerEvent *event);

private:
    GLTransformStack &transformStack()
    { return m_gl_state.transformStack(); }

private:
    GLState m_gl_state;
    bool m_is_animating;
    int m_animation_frame;
    int stop;
    UnitSquare m_unit_square;
    UnitCircle m_unit_circle;
    Body body;
    Head head;
    Mouth mouth;
    Arm arm;
    //////////////////////////////////////////////////////////////////////////
    // TODO: Add additional joint parameters.
    //////////////////////////////////////////////////////////////////////////
    double arm_angle;
    double head_angle;
    double mouth_angle;
    double left_leg_angle;
    double left_feet_angle;
    double right_leg_angle;
    double right_feet_angle;

    // for animation
    double xmove;
    double ymove;
    double hmove;
};

#endif
