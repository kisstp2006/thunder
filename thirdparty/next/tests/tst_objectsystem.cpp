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

#include "objectsystem.h"

#include "json.h"
#include "bson.h"

class SecondObject : public TestObject {
    A_REGISTER(SecondObject, TestObject, Test)

    A_NOMETHODS()
    A_NOPROPERTIES()

};

class TestObjectEx : public TestObject {
    A_OVERRIDE(TestObjectEx, TestObject, Test)

    A_NOMETHODS()
    A_NOPROPERTIES()
};

class ObjectSystemTest : public QObject {
    Q_OBJECT
private slots:

void RegisterUnregister_Object() {
    ObjectSystem objectSystem;

    QCOMPARE((int)objectSystem.factories().size(), 0);
    SecondObject::registerClassFactory(&objectSystem);
    QCOMPARE((int)objectSystem.factories().size(), 1);
    SecondObject::unregisterClassFactory(&objectSystem);
    QCOMPARE((int)objectSystem.factories().size(), 0);
}

void Object_Instansing() {
    ObjectSystem objectSystem;
    TestObject::registerClassFactory(&objectSystem);

    TestObject obj1;
    Object *result1 = ObjectSystem::objectCreate<TestObject>();
    Object *object  = dynamic_cast<Object*>(&obj1);

    QCOMPARE((result1 != nullptr), true);
    QCOMPARE((object != nullptr), true);
    QCOMPARE(compare(*object, *result1), true);

    Object *result2 = ObjectSystem::objectCreate<TestObject>();

    QCOMPARE((result1->uuid() != result2->uuid()), true);

    delete result1;
    delete result2;
}

void Override_Object() {
    ObjectSystem objectSystem;
    TestObject::registerClassFactory(&objectSystem);

    TestObjectEx::registerClassFactory(&objectSystem);

    Object *object = ObjectSystem::objectCreate<TestObject>();

    QCOMPARE((object != nullptr), true);
    const MetaObject *meta = object->metaObject();

    QCOMPARE((dynamic_cast<TestObjectEx *>(object) != nullptr), true);
    QCOMPARE(meta->methodCount(), 7);
    QCOMPARE(meta->propertyCount(), 4);

    int index = meta->indexOfProperty("slot");
    QCOMPARE((index > -1), true);
    delete object;

    TestObjectEx::unregisterClassFactory(&objectSystem);

    object = ObjectSystem::objectCreate<TestObject>();
    QCOMPARE((dynamic_cast<TestObject *>(object) != nullptr), true);
    QCOMPARE((dynamic_cast<TestObjectEx *>(object) == nullptr), true);
    delete object;
}

void Serialize_Desirialize_Object() {
    ObjectSystem objectSystem;
    TestObject::registerClassFactory(&objectSystem);

    TestObject *obj1 = ObjectSystem::objectCreate<TestObject>();
    TestObject *obj2 = ObjectSystem::objectCreate<TestObject>();
    TestObject *obj3 = ObjectSystem::objectCreate<TestObject>();

    obj1->setName("MainObject");

    obj2->setName("TestComponent2");
    obj3->setName("TestComponent3");
    obj2->setParent(obj1);
    obj3->setParent(obj2);

    QCOMPARE(Object::connect(obj1, _SIGNAL(signal(int)), obj2, _SLOT(setSlot(int))), true);
    QCOMPARE(Object::connect(obj1, _SIGNAL(signal(int)), obj3, _SIGNAL(signal(int))), true);
    QCOMPARE(Object::connect(obj2, _SIGNAL(signal(int)), obj3, _SLOT(setSlot(int))), true);

    ByteArray bytes = Bson::save(ObjectSystem::toVariant(obj1));
    Object *result  = ObjectSystem::toObject(Bson::load(bytes));
    Object *object  = dynamic_cast<Object*>(obj1);

    QCOMPARE((result != nullptr), true);
    QCOMPARE((object != nullptr), true);
    QCOMPARE(compare(*object, *result), true);

    QCOMPARE((object->getReceivers().size() == result->getReceivers().size()), true);

    QCOMPARE((object->uuid() == result->uuid()), true);

    delete result;

    delete obj3;
    delete obj2;
    delete obj1;
}

} REGISTER(ObjectSystemTest)

#include "tst_objectsystem.moc"
