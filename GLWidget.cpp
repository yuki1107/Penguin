/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1

    Instructions:
        See main.cpp for more instructions.

        This file contains the class OpenGL portion of the main window.
**************************************************************************/

#include "GLWidget.h"
#include <iostream>

static QGLFormat createQGLFormat()
{
    // Return a QGLFormat object that tells Qt information about what
    // OpenGL context we would like.
    QGLFormat format(QGL::SampleBuffers);
    if (supportsNewOpenGL())
    {
	// Choose a version of OpenGL that supports vertex array objects and
	// tell it that we do not want support for deprecated functions.
        format.setVersion(3, 3);
        format.setProfile(QGLFormat::CoreProfile);
    }
    return format;
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(createQGLFormat(), parent)
    , m_is_animating(false)
    , m_animation_frame(0)
    , arm_angle(0)
    , head_angle(0)
    , mouth_angle(0)
    , left_leg_angle(0)
    , left_feet_angle(0)
    , right_leg_angle(0)
    , right_feet_angle(0)
    , hmove(0)
{
    // Start a timer that will call the timerEvent method every 50ms.
    startTimer(/*milliseconds=*/50);
}

void GLWidget::initializeGL()
{
    m_gl_state.initializeGL();

    // To aid with troubleshooting, print out which version of OpenGL we've
    // told the driver to use.
    std::cout << "Using OpenGL: " << glGetString(GL_VERSION) << std::endl;

    // Copy the data for the shapes we'll draw into the video card's memory.
    arm.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    mouth.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    head.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    body.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    m_unit_square.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION);
    m_unit_circle.initialize(m_gl_state.VERTEX_POSITION_SHADER_LOCATION,
	/*num_circle_segments=*/100);

    // Tell OpenGL what color to fill the background to when clearing.
    glClearColor(/*red=*/0.7f, /*green=*/0.7f, /*blue=*/0.9f, /*alpha=*/1.0f);

}

void GLWidget::resizeGL(int width, int height)
{
    // Respond to the window being resized by updating the viewport and
    // projection matrices.

    checkForGLErrors();

    // Setup projection matrix for new window
    m_gl_state.setOrthographicProjectionFromWidthAndHeight(width, height);

    // Update OpenGL viewport and internal variables
    glViewport(0, 0, width, height);
    checkForGLErrors();
}

void GLWidget::timerEvent(QTimerEvent *)
{
    // Respond to a timer going off telling us to update the animation.
    if (!m_is_animating)
        return;

    // increment the frame number.
    m_animation_frame++;

    // Update joint angles.
    const double joint_rot_speed = 0.1;
    double joint_rot_t =
        (sin(m_animation_frame * joint_rot_speed) + 1.0) / 2.0;
    head_angle = joint_rot_t * HEAD_MAX + (1 - joint_rot_t) * HEAD_MIN;
    arm_angle = joint_rot_t * ARM_MIN + (1 - joint_rot_t) * ARM_MAX;
    left_leg_angle = joint_rot_t * ARM_MIN + (1 - joint_rot_t) * ARM_MAX;
    right_leg_angle = joint_rot_t * ARM_MIN + (1 - joint_rot_t) * ARM_MAX;
    left_feet_angle = joint_rot_t * ARM_MIN + (1 - joint_rot_t) * ARM_MAX;
    right_feet_angle = joint_rot_t * ARM_MIN + (1 - joint_rot_t) * ARM_MAX;   
    mouth_angle =  joint_rot_t * MOUTH_MIN + (1 - joint_rot_t) * MOUTH_MAX;
    
    xmove = -1*(joint_rot_t * ARM_MIN + (1 - joint_rot_t) * ARM_MAX);
    ymove = -1*(joint_rot_t * ARM_MAX + (1 - joint_rot_t) * ARM_MIN);
    if (ymove < -30){
        stop++;
    }
    hmove = -1 * pow(((m_animation_frame - stop) % 180) * joint_rot_speed, 2) * 5 + 600;

    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function to animate the character's joints
    //   Note: Nothing should be drawn in this function!
    //////////////////////////////////////////////////////////////////////////

    // Tell this widget to redraw itself.
    update();
}

void GLWidget::paintGL()
{
    // This method gets called by the event handler to draw the scene, so
    // this is where you need to build your scene -- make your changes and
    // additions here.  All rendering happens in this function.

    checkForGLErrors();

    // Clear the screen with the background colour.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

    // Setup the model-view transformation matrix stack.
    transformStack().loadIdentity();
    checkForGLErrors();

    //////////////////////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function draw the scene.  This should include function
    //   calls to pieces that apply the appropriate transformation matrices
    //   and render the individual body parts.
    //////////////////////////////////////////////////////////////////////////

    // Draw our hinged object
    const float BODY_WIDTH = 60.0f;
    const float BODY_LENGTH = 80.0f;
    const float ARM_LENGTH = 100.0f;
    const float ARM_WIDTH = 45.0f;
    const float HEAD_WIDTH = 80.0f;
    const float HEAD_LENGTH = 35.0f;
    const float LEG_LENGTH = 75.0f;
    const float LEG_WIDTH = 25.0f;
    const float FEET_LENGTH = 50.0f;
    const float FEET_WIDTH = 15.0f;


    // Note that successive transformations are applied *before* the previous
    // ones.

    // Push the current transformation matrix on the stack
    transformStack().pushMatrix();
        // Movement of the whole penguin
        transformStack().translate(hmove, 0);        
        transformStack().translate(xmove, ymove);

        // Draw the 'body' *******************************************************
        transformStack().pushMatrix();
            // Scale square to size of body
            transformStack().scale(BODY_WIDTH, BODY_LENGTH);

            // Set the colour to green
            m_gl_state.setColor(255, 255, 255);

            // Draw the square for the body
            body.draw();
        transformStack().popMatrix();

        // Draw the 'back leg' **************************************************
        transformStack().pushMatrix();
            transformStack().translate(30.0, -1 * BODY_LENGTH);
            transformStack().rotateInDegrees(right_leg_angle);
            transformStack().translate(0.0, -0.35 * LEG_LENGTH);
            
            // 'leg'
            transformStack().pushMatrix();
                m_gl_state.setColor(0.0, 0.0, 0.0);
                transformStack().scale(LEG_WIDTH, LEG_LENGTH);
                m_unit_square.draw();
            transformStack().popMatrix();

            // 'feet'
            transformStack().pushMatrix();
                transformStack().translate(0.0, -0.5 * (LEG_LENGTH - FEET_WIDTH));
                // Draw the feet
                transformStack().pushMatrix();
                    transformStack().rotateInDegrees(right_feet_angle);
                    transformStack().rotateInDegrees(30);
                    transformStack().translate(-0.35 * FEET_LENGTH, 0.0);
                    m_gl_state.setColor(255, 255, 0.0);
                    transformStack().scale(FEET_LENGTH, FEET_WIDTH);
                    m_unit_square.draw();
                transformStack().popMatrix();

                // Draw joint circle for feet
                transformStack().pushMatrix();
                    transformStack().scale(6.0f, 6.0f);
                    m_gl_state.setColor(255, 0, 0);
                    m_unit_circle.draw();
                transformStack().popMatrix();
            transformStack().popMatrix();

            // Draw joint circle for leg
            transformStack().pushMatrix();
                transformStack().translate(0.0, 0.35 * LEG_LENGTH);
                transformStack().scale(6.0f, 6.0f);
                m_gl_state.setColor(255, 0, 0);
                m_unit_circle.draw();
            transformStack().popMatrix();
        transformStack().popMatrix();

        // Draw the 'front leg' **************************************************
        transformStack().pushMatrix();
            transformStack().translate(-30.0, -1 * BODY_LENGTH);
            transformStack().rotateInDegrees(left_leg_angle);
            transformStack().translate(0.0, -0.35 * LEG_LENGTH);

            // 'leg'
            transformStack().pushMatrix();
                m_gl_state.setColor(0.0, 0.0, 0.0);
                transformStack().scale(LEG_WIDTH, LEG_LENGTH);
                m_unit_square.draw();
            transformStack().popMatrix();

            // 'feet'
            transformStack().pushMatrix();
                transformStack().translate(0.0, -0.5 * (LEG_LENGTH - FEET_WIDTH));
                // Draw the feet
                transformStack().pushMatrix();
                    transformStack().rotateInDegrees(left_feet_angle);
                    transformStack().rotateInDegrees(30);
                    transformStack().translate(-0.35 * FEET_LENGTH, 0.0);
                    m_gl_state.setColor(255, 255, 0.0);
                    transformStack().scale(FEET_LENGTH, FEET_WIDTH);
                    m_unit_square.draw();
                transformStack().popMatrix();

                // Draw joint circle for feet
                transformStack().pushMatrix();
                    transformStack().scale(6.0f, 6.0f);
                    m_gl_state.setColor(255, 0, 0);
                    m_unit_circle.draw();
                transformStack().popMatrix();
            transformStack().popMatrix();

            

            // Draw joint circle for leg
            transformStack().pushMatrix();
                transformStack().translate(0.0, 0.35 * LEG_LENGTH);
                transformStack().scale(6.0f, 6.0f);
                m_gl_state.setColor(255, 0, 0);
                m_unit_circle.draw();
            transformStack().popMatrix();
        transformStack().popMatrix();

        // Draw the 'arm' ********************************************************
        transformStack().pushMatrix();
            // Move the arm to the joint hinge
            transformStack().translate(15.0, ARM_WIDTH * 1.5);
            
            transformStack().pushMatrix();    
                // Rotate along the hinge
                transformStack().rotateInDegrees(arm_angle);
                // Scale the size of the arm
                transformStack().scale(ARM_WIDTH, ARM_LENGTH);
                // Move to center location of arm, under previous rotation
                transformStack().translate(0.0, -0.35);
                // Draw the square for the arm
                m_gl_state.setColor(0.0, 0.0, 0.0);
        	    arm.draw();
            transformStack().popMatrix();

            // Draw joint circle for arm
            transformStack().pushMatrix();
                transformStack().translate(0.0, 0.35);
                transformStack().scale(6.0f, 6.0f);
                m_gl_state.setColor(255, 0, 0);
                m_unit_circle.draw();
            transformStack().popMatrix();
        transformStack().popMatrix();

        // Draw the 'head' ********************************************************
        transformStack().pushMatrix();
            transformStack().translate(0.0, BODY_LENGTH * 1.7 - HEAD_LENGTH / 2);
            transformStack().rotateInDegrees(head_angle);
            transformStack().translate(0.0, HEAD_LENGTH / 2);
            
            // Draw the 'head' shape
            transformStack().pushMatrix();
                transformStack().scale(HEAD_WIDTH, HEAD_LENGTH);
                m_gl_state.setColor(0.0, 0.0, 0.0);
                head.draw();
            transformStack().popMatrix();

            // Draw joint circle for head
            transformStack().pushMatrix();
                transformStack().translate(0.0, HEAD_LENGTH / -2);
                transformStack().scale(6.0f, 6.0f);
                m_gl_state.setColor(255, 0, 0);
                m_unit_circle.draw();
            transformStack().popMatrix();

            // Draw the 'eye'
            transformStack().pushMatrix();
                transformStack().translate(-0.2*HEAD_WIDTH, 0.5 * HEAD_LENGTH);
                // 'eye white'
                transformStack().pushMatrix();
                    transformStack().scale(8.0f, 8.0f);
                    m_gl_state.setColor(255, 255, 255);
                    m_unit_circle.draw();
                transformStack().popMatrix();
                // 'eyeball'
                transformStack().pushMatrix();
                    transformStack().translate(-3.0, 0.0);
                    transformStack().scale(5.0f, 5.0f);
                    m_gl_state.setColor(0.0, 0.0, 0.0);
                    m_unit_circle.draw();
                transformStack().popMatrix();
            transformStack().popMatrix();

            // Draw the 'mouth'
            transformStack().pushMatrix();
                transformStack().translate(-0.8*HEAD_WIDTH, -5.0);
                // 'upper lip'
                transformStack().pushMatrix();
                    transformStack().scale(HEAD_WIDTH * 0.8, HEAD_LENGTH);
                    m_gl_state.setColor(255, 255, 0.0);
                    mouth.draw();
                transformStack().popMatrix();

                // underlip
                transformStack().pushMatrix();
                    transformStack().translate(0.0, -10.0);
                    transformStack().translate(0.0, mouth_angle);
                    transformStack().scale(HEAD_WIDTH * 0.8, HEAD_LENGTH / 8);
                    m_gl_state.setColor(255, 255, 0.0);
                    m_unit_square.draw();
                transformStack().popMatrix();
            transformStack().popMatrix(); 

        transformStack().popMatrix();


    // Retrieve the previous state of the transformation stack
    transformStack().popMatrix();


    // Execute any GL functions that are in the queue just to be safe
    glFlush();
    checkForGLErrors();
}

