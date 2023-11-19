/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    Copyright: 2008-2023 Evgeniy Prikazchikov
*/

#include "tst_common.h"

#include "anim/propertyanimation.h"
#include "anim/variantanimation.h"

class AnimationTest : public QObject {
    Q_OBJECT
private slots:

void Base_animation() {
    Animation anim;
}

void Property_animation() {
    PropertyAnimation anim;
    TestObject object;
    anim.setTarget(&object, "vec");
    QCOMPARE((anim.target() != nullptr), true);

    AnimationCurve curveX;

    AnimationCurve::KeyFrame x1;
    x1.m_Value = 0.0f;
    x1.m_Position = 0.0f;
    x1.m_Type = AnimationCurve::KeyFrame::Linear;
    curveX.m_Keys.push_back(x1);

    AnimationCurve::KeyFrame x2;
    x2.m_Value = 1.0f;
    x2.m_Position = 1.0f;
    x2.m_Type = AnimationCurve::KeyFrame::Linear;
    curveX.m_Keys.push_back(x2);

    anim.setCurve(&curveX, 0);

    AnimationCurve curveY;

    AnimationCurve::KeyFrame y1;
    y1.m_Value = 0.0f;
    y1.m_Position = 0.0f;
    y1.m_Type = AnimationCurve::KeyFrame::Linear;
    curveY.m_Keys.push_back(y1);

    AnimationCurve::KeyFrame y2;
    y2.m_Value = 2.0f;
    y2.m_Position = 1.0f;
    y2.m_Type = AnimationCurve::KeyFrame::Linear;
    curveY.m_Keys.push_back(y2);

    anim.setCurve(&curveY, 1);
    anim.setDuration(1000);
    anim.start();

    anim.setCurrentTime(500);

    QCOMPARE(object.getVector(), Vector2(0.5, 1.0f));
}

} REGISTER(AnimationTest)

#include "tst_animation.moc"
