#include "Free_OpenGLWidget.h"
#include "Controller.h"
#include <QDebug>
#include <cmath>
#include <QtMath>


Free_OpenGLWidget::Free_OpenGLWidget(Model *model, Controller *controller, QWidget *parent)
    : View_OpenGLWidget(model, controller, parent),
      yaw(0.0f), pitch(0.0f), isTilting(false)
{
    // Initial camera setup
    cameraTarget = QVector3D(0, 0, 0);    // Look at center
    cameraUp = QVector3D(0, 1, 0);        // Y is up
    cameraPosition = QVector3D(0.0f, 0.0f, cameraDistance); // Start position at fixed distance
}

Free_OpenGLWidget::~Free_OpenGLWidget()
{
}

void Free_OpenGLWidget::UpdateMatrices()
{
    float w = width();
    float h = height();
    
    // Create perspective projection matrix with fixed FOV
    float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
    float fov = 45.0f; // Fixed FOV for more natural perspective
    
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fov, aspectRatio, 0.1f, 1000.0f);
    
    // Calculate direction vectors for camera orientation
    float yawRad = qDegreesToRadians(yaw);
    float pitchRad = qDegreesToRadians(pitch);
    
    QVector3D front;
    front.setX(cos(yawRad) * cos(pitchRad));
    front.setY(sin(pitchRad));
    front.setZ(sin(yawRad) * cos(pitchRad));
    front.normalize();
    
    // Calculate camera position at fixed distance
    cameraPosition = cameraTarget - front * cameraDistance;
    
    // Create view matrix
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);
    
    // Apply regular panning as translation of target point (and camera follows)
    if (!isTilting) {
        QVector3D right = QVector3D::crossProduct(front, cameraUp).normalized();
        QVector3D up = QVector3D::crossProduct(right, front).normalized();
        
        // Move both camera and target to maintain relative position
        float panSpeed = 0.01f * cameraDistance; // Use fixed distance for pan speed
        QVector3D panOffset = right * panOffsetX * panSpeed + up * panOffsetY * panSpeed;
        
        cameraTarget += panOffset;
        cameraPosition += panOffset;
        
        // Reset panning offsets after applying them
        panOffsetX = 0.0f;
        panOffsetY = 0.0f;
        
        // Recalculate view matrix with new positions
        viewMatrix.setToIdentity();
        viewMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);
    }
}

// Override wheelEvent to disable zooming
void Free_OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    // Do nothing - zooming is disabled
    event->accept(); // Accept the event to prevent it from being passed to parent
}

QVector3D Free_OpenGLWidget::screenToWorld(int x, int y)
{
    // Convert screen coordinates to normalized device coordinates
    float normalizedX = (2.0f * x / width()) - 1.0f;
    float normalizedY = 1.0f - (2.0f * y / height());
    
    // Create a ray in view space
    QVector4D rayClip(normalizedX, normalizedY, -1.0f, 1.0f);
    QVector4D rayEye = projectionMatrix.inverted() * rayClip;
    rayEye = QVector4D(rayEye.x(), rayEye.y(), -1.0f, 0.0f);
    
    // Transform ray to world space
    QVector4D rayWorld = viewMatrix.inverted() * rayEye;
    QVector3D rayDirection(rayWorld.x(), rayWorld.y(), rayWorld.z());
    rayDirection.normalize();
    
    // Compute intersection with XY plane (z = 0)
    float t = -cameraPosition.z() / rayDirection.z();
    QVector3D intersection = cameraPosition + rayDirection * t;
    
    return intersection;
}

void Free_OpenGLWidget::updateBuffers()
{
    View_OpenGLWidget::updateBuffers();
    
    // Create a 3D grid for the free camera view
    float gridExtent = 10.0f * gridSize;
    int gridLines = 21;  // Center line + 10 on each side
    float step = gridExtent / (gridLines - 1);
    
    std::vector<float> newGridData;
    
    // XY plane grid
    for (int i = 0; i < gridLines; i++) {
        float pos = -gridExtent / 2.0f + i * step;
        
        // X axis line (along X, constant Y)
        newGridData.push_back(-gridExtent / 2.0f);
        newGridData.push_back(pos);
        newGridData.push_back(0.0f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        newGridData.push_back(0.5f);
        
        newGridData.push_back(gridExtent / 2.0f);
        newGridData.push_back(pos);
        newGridData.push_back(0.0f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        newGridData.push_back(0.5f);
        
        // Y axis line (constant X, along Y)
        newGridData.push_back(pos);
        newGridData.push_back(-gridExtent / 2.0f);
        newGridData.push_back(0.0f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        newGridData.push_back(0.5f);
        
        newGridData.push_back(pos);
        newGridData.push_back(gridExtent / 2.0f);
        newGridData.push_back(0.0f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(0.5f);
        newGridData.push_back(1.0f);
        newGridData.push_back(0.5f);
    }
    
    // Add coordinate axes (thicker lines)
    // X-axis (red)
    newGridData.push_back(-gridExtent / 2.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f); // Red
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(2.0f); // Thicker
    
    newGridData.push_back(gridExtent / 2.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(2.0f);
    
    // Y-axis (green)
    newGridData.push_back(0.0f);
    newGridData.push_back(-gridExtent / 2.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f); // Green
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(2.0f);
    
    newGridData.push_back(0.0f);
    newGridData.push_back(gridExtent / 2.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(2.0f);
    
    // Z-axis (blue)
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(-gridExtent / 2.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f); // Blue
    newGridData.push_back(1.0f);
    newGridData.push_back(2.0f);
    
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(gridExtent / 2.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(0.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(1.0f);
    newGridData.push_back(2.0f);
    
    gridData = newGridData;
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridData.size() * sizeof(float), gridData.data(), GL_DYNAMIC_DRAW);
}

void Free_OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        lastPanPosition = event->pos();
        
        if (ctrlPressed) {
            // Tilting mode
            isTilting = true;
            setCursor(Qt::SizeAllCursor);
        } else {
            // Regular panning mode
            isPanning = true;
            setCursor(Qt::ClosedHandCursor);
        }
    } else if (controller) {
        controller->handleMouseClick(event);
    }
}

void Free_OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isTilting) {
        QPoint delta = event->pos() - lastPanPosition;
        
        // Update yaw and pitch based on mouse movement
        float sensitivity = 0.2f;
        yaw += delta.x() * sensitivity;
        pitch -= delta.y() * sensitivity;
        
        lastPanPosition = event->pos();
        
        UpdateMatrices();
        update();
    } else if (isPanning) {
        QPoint delta = event->pos() - lastPanPosition;
        
        // Scale sensitivity for better control
        float sensitivity = 1.0f;
        panOffsetX += delta.x() * sensitivity;
        panOffsetY -= delta.y() * sensitivity;
        
        lastPanPosition = event->pos();
        
        UpdateMatrices();
        update();
    } else if (controller) {
        controller->handleMouseMove(event);
    }
}

void Free_OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        isPanning = false;
        isTilting = false;
        setCursor(Qt::ArrowCursor);
    }
}
