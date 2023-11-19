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

#include "threadpool.h"

class ThreadObject : public Object {
public:
    explicit ThreadObject() :
        Object(),
        m_counter(0) {

    }

    void post() {
        Event *e = new Event(Event::UserType);
        postEvent(e);
    }

    bool event(Event *e) {
        if(e->type() == Event::UserType) {
            QTest::qSleep(1000);
            m_counter++;
            return true;
        }
        return false;
    }

    uint32_t counter() const {
        return m_counter;
    }

    uint32_t m_counter;
};

class TreadPoolTest : public QObject {
    Q_OBJECT
private slots:

void Multi_Task() {
    qDebug() << "Optimal number of threads =" << ThreadPool::optimalThreadCount();

    ThreadPool pool;

    ThreadObject obj;
    obj.setName("MainObject");
    for(int i = 0; i < 16; i++) {
        ThreadObject *object = new ThreadObject();
        object->setName(string("TestComponent") + to_string(i));
        object->setParent(&obj);
        object->post();
        pool.start(*object);
    }
    pool.waitForDone();

    for(auto it : obj.findChildren<ThreadObject *>()) {
        QCOMPARE(it->counter(), uint32_t(1));
    }
}

} REGISTER(ThreadPool)

#include "tst_threadpool.moc"
