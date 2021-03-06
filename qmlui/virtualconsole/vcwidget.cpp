/*
  Q Light Controller Plus
  vcwidget.cpp

  Copyright (c) Massimo Callegari

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/


#include <QtXml>

#include "vcwidget.h"
#include "doc.h"

VCWidget::VCWidget(Doc *doc, QObject *parent)
    : QObject(parent)
    , m_doc(doc)
    , m_id(invalidId())
    , m_type(UnknownWidget)
    , m_geometry(QRect(0,0,0,0))
    , m_allowResize(true)
    , m_isDisabled(false)
    , m_isVisible(true)
    , m_caption(QString())
    , m_backgroundColor(QColor("#333"))
    , m_hasCustomBackgroundColor(false)
    , m_foregroundColor(QColor(Qt::black))
    , m_hasCustomForegroundColor(false)
    , m_page(0)
{
    m_font = QFont("RobotoCondensed");
    m_font.setPointSize(10);
}

VCWidget::~VCWidget()
{
}

void VCWidget::setDocModified()
{
    if (m_doc != NULL)
        m_doc->setModified();
}

void VCWidget::render(QQuickView *, QQuickItem *)
{
}

/*****************************************************************************
 * ID
 *****************************************************************************/

void VCWidget::setID(quint32 id)
{
    /* Don't set doc modified status or emit changed signal, because this
       function is called only once during widget creation. */
    m_id = id;
}

quint32 VCWidget::id() const
{
    return m_id;
}

quint32 VCWidget::invalidId()
{
    return UINT_MAX;
}

/*********************************************************************
 * Type
 *********************************************************************/

void VCWidget::setType(int type)
{
    m_type = type;
}

int VCWidget::type()
{
    return m_type;
}

QString VCWidget::typeToString(int type)
{
    switch (type)
    {

        case ButtonWidget: return QString(tr("Button"));
        case SliderWidget: return QString(tr("Slider"));
        case FrameWidget: return QString(tr("Frame"));
        case SoloFrameWidget: return QString(tr("Solo Frame"));
        case SpeedDialWidget: return QString(tr("Speed Dial"));
        case XYPadWidget: return QString(tr("XY Pad"));
        case CueListWidget: return QString(tr("Cue list"));
        case LabelWidget: return QString(tr("Label"));
        case AudioTriggersWidget: return QString(tr("Audio Triggers"));
        case AnimationWidget: return QString(tr("Animation"));
        case UnknownWidget:
        default:
             return QString(tr("Unknown"));
    }
    return QString(tr("Unknown"));
}

QString VCWidget::typeToIcon(int type)
{
    switch (type)
    {
        case ButtonWidget: return QString("qrc:/button.svg");
        case SliderWidget: return QString("qrc:/slider.svg");
        case FrameWidget: return QString("qrc:/frame.svg");
        case SoloFrameWidget: return QString("qrc:/soloframe.svg");
        case SpeedDialWidget: return QString("qrc:/speed.svg");
        case XYPadWidget: return QString("qrc:/xypad.svg");
        case CueListWidget: return QString("qrc:/cuelist.svg");
        case LabelWidget: return QString("qrc:/label.svg");
        case AudioTriggersWidget: return QString("qrc:/audioinput.svg");
        case AnimationWidget: return QString("qrc:/rgbmatrix.svg");
        case ClockWidget: return QString("qrc:/clock.svg");
        case UnknownWidget:
        default:
             return QString("qrc:/virtualconsole.svg");
    }
    return QString("qrc:/virtualconsole.svg");
}

VCWidget::WidgetType VCWidget::stringToType(QString str)
{
    if (str == "Button") return ButtonWidget;
    else if (str == "Slider") return SliderWidget;
    else if (str == "XYPad") return XYPadWidget;
    else if (str == "Frame") return FrameWidget;
    else if (str == "Solo frame") return SoloFrameWidget;
    else if (str == "Speed dial") return SpeedDialWidget;
    else if (str == "Cue list") return CueListWidget;
    else if (str == "Label") return LabelWidget;
    else if (str == "Audio Triggers") return AudioTriggersWidget;
    else if (str == "Animation") return AnimationWidget;
    else if (str == "Clock") return ClockWidget;

    return UnknownWidget;
}

/*********************************************************************
 * Geometry
 *********************************************************************/

QRect VCWidget::geometry() const
{
    return m_geometry;
}

void VCWidget::setGeometry(QRect rect)
{
    if (m_geometry == rect)
        return;

    m_geometry = rect;
    setDocModified();
    emit geometryChanged(rect);
}

/*********************************************************************
 * Allow resize
 *********************************************************************/

bool VCWidget::allowResize() const
{
    return m_allowResize;
}

void VCWidget::setAllowResize(bool allowResize)
{
    if (m_allowResize == allowResize)
        return;

    m_allowResize = allowResize;
    emit allowResizeChanged(allowResize);
}

/*********************************************************************
 * Disable state
 *********************************************************************/

bool VCWidget::isDisabled()
{
    return m_isDisabled;
}

void VCWidget::setDisabled(bool disable)
{
    if (m_isDisabled == disable)
        return;

    m_isDisabled = disable;
    setDocModified();
    emit disabledStateChanged(disable);
}

/*********************************************************************
 * Visibility state
 *********************************************************************/

void VCWidget::setVisible(bool isVisible)
{
    if (m_isVisible == isVisible)
        return;

    m_isVisible = isVisible;
    emit isVisibleChanged(isVisible);
}

bool VCWidget::isVisible() const
{
    return m_isVisible;
}

/*****************************************************************************
 * Caption
 *****************************************************************************/

QString VCWidget::caption() const
{
    return m_caption;
}

void VCWidget::setCaption(QString caption)
{
    if (m_caption == caption)
        return;

    m_caption = caption;
    setDocModified();
    emit captionChanged(caption);
}

/*****************************************************************************
 * Background color
 *****************************************************************************/

QColor VCWidget::backgroundColor() const
{
    return m_backgroundColor;
}

void VCWidget::setBackgroundColor(QColor backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    m_backgroundColor = backgroundColor;
    m_hasCustomBackgroundColor = true;
    setDocModified();
    emit backgroundColorChanged(backgroundColor);
}

bool VCWidget::hasCustomBackgroundColor() const
{
    return m_hasCustomBackgroundColor;
}

void VCWidget::resetBackgroundColor()
{
    m_hasCustomBackgroundColor = false;
    m_backgroundColor = Qt::gray;
    setDocModified();
    emit backgroundColorChanged(m_backgroundColor);
}

/*****************************************************************************
 * Foreground color
 *****************************************************************************/

QColor VCWidget::foregroundColor() const
{
    return m_foregroundColor;
}

void VCWidget::setForegroundColor(QColor foregroundColor)
{
    if (m_foregroundColor == foregroundColor)
        return;

    m_foregroundColor = foregroundColor;
    m_hasCustomForegroundColor = true;
    setDocModified();
    emit foregroundColorChanged(foregroundColor);
}

bool VCWidget::hasCustomForegroundColor() const
{
    return m_hasCustomForegroundColor;
}

void VCWidget::resetForegroundColor()
{
    m_hasCustomForegroundColor = false;
    m_foregroundColor = Qt::black;
    setDocModified();
    emit foregroundColorChanged(m_foregroundColor);
}

/*********************************************************************
 * Font
 *********************************************************************/

void VCWidget::setFont(const QFont& font)
{
    m_hasCustomFont = true;
    m_font = font;
    setDocModified();
    emit fontChanged();
}

QFont VCWidget::font() const
{
    return m_font;
}

bool VCWidget::hasCustomFont() const
{
    return m_hasCustomFont;
}

void VCWidget::resetFont()
{
    m_font = QFont("RobotoCondensed");
    m_font.setPointSize(16);
    m_hasCustomFont = false;
    setDocModified();
    emit fontChanged();
}

/*********************************************************************
 * Page
 *********************************************************************/

void VCWidget::setPage(int pNum)
{
    if (pNum == m_page)
        return;

    m_page = pNum;
    emit pageChanged(pNum);
}

int VCWidget::page()
{
    return m_page;
}

/*****************************************************************************
 * Load & Save
 *****************************************************************************/

bool VCWidget::loadXML(const QDomElement *)
{
    return false;
}

bool VCWidget::loadXMLCommon(const QDomElement *root)
{
    Q_ASSERT(root != NULL);

    /* ID */
    if (root->hasAttribute(KXMLQLCVCWidgetID))
        setID(root->attribute(KXMLQLCVCWidgetID).toUInt());

    /* Caption */
    if (root->hasAttribute(KXMLQLCVCCaption))
        setCaption(root->attribute(KXMLQLCVCCaption));

    /* Page */
    if (root->hasAttribute(KXMLQLCVCWidgetPage))
        setPage(root->attribute(KXMLQLCVCWidgetPage).toInt());

    return true;
}

bool VCWidget::loadXMLAppearance(const QDomElement* root)
{
    QDomNode node;
    QDomElement tag;

    Q_ASSERT(root != NULL);

    if (root->tagName() != KXMLQLCVCWidgetAppearance)
    {
        qWarning() << Q_FUNC_INFO << "Appearance node not found!";
        return false;
    }

    /* Children */
    node = root->firstChild();
    while (node.isNull() == false)
    {
        tag = node.toElement();
        if (tag.tagName() == KXMLQLCVCWidgetForegroundColor)
        {
            if (tag.text() != KXMLQLCVCWidgetColorDefault)
                setForegroundColor(QColor(tag.text().toUInt()));
            else if (hasCustomForegroundColor() == true)
                resetForegroundColor();
        }
        else if (tag.tagName() == KXMLQLCVCWidgetBackgroundColor)
        {
            if (tag.text() != KXMLQLCVCWidgetColorDefault)
                setBackgroundColor(QColor(tag.text().toUInt()));
        }
/*
        else if (tag.tagName() == KXMLQLCVCWidgetBackgroundImage)
        {
            if (tag.text() != KXMLQLCVCWidgetBackgroundImageNone)
                setBackgroundImage(m_doc->denormalizeComponentPath(tag.text()));
        }
*/
        else if (tag.tagName() == KXMLQLCVCWidgetFont)
        {
            if (tag.text() != KXMLQLCVCWidgetFontDefault)
            {
                QFont font;
                font.fromString(tag.text());
                setFont(font);
            }
        }
        else if (tag.tagName() == KXMLQLCVCFrameStyle)
        {
            /** LEGACY: no more supported/needed */
        }
        else
        {
            qWarning() << Q_FUNC_INFO << "Unknown appearance tag:" << tag.tagName();
        }

        node = node.nextSibling();
    }

    return true;
}

bool VCWidget::loadXMLWindowState(const QDomElement* tag, int* x, int* y,
                                  int* w, int* h, bool* visible)
{
    if (tag == NULL || x == NULL || y == NULL || w == NULL || h == NULL ||
            visible == NULL)
        return false;

    if (tag->tagName() == KXMLQLCWindowState)
    {
        *x = tag->attribute(KXMLQLCWindowStateX).toInt();
        *y = tag->attribute(KXMLQLCWindowStateY).toInt();
        *w = tag->attribute(KXMLQLCWindowStateWidth).toInt();
        *h = tag->attribute(KXMLQLCWindowStateHeight).toInt();

        if (tag->attribute(KXMLQLCWindowStateVisible) == KXMLQLCTrue)
            *visible = true;
        else
            *visible = false;

        return true;
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Window state not found";
        return false;
    }
}
