//
//  dispatch.cpp
//  ipso-hello
//
//  Copyright ipsosanté 2014. All rights reserved.
//

#include "dispatch.hh"

#include <QMetaObject>
#include <QCoreApplication>
#include <QObject>
#include <QWidget>

static QThread* s_dispatch_thread = NULL;

void dispatchAsyncMain(FnBlock block)
{
  qRegisterMetaType<FnBlock>("FnBlock");

  QThread *mainThread = QCoreApplication::instance()->thread();
  WorkerClass *worker = new WorkerClass(mainThread);
  QMetaObject::invokeMethod(worker,
                            "doWork",
                            Qt::QueuedConnection,
                            Q_ARG(FnBlock, block));
}

void dispatchAsync(QThread* thread, FnBlock block)
{
  qRegisterMetaType<FnBlock>("FnBlock");

  WorkerClass *worker = new WorkerClass(thread);
  QMetaObject::invokeMethod(worker,
                            "doWork",
                            Qt::QueuedConnection,
                            Q_ARG(FnBlock, block));
}

void dispatchAsync(FnBlock block)
{
  if (!s_dispatch_thread)
  {
      s_dispatch_thread = new QThread();
      s_dispatch_thread->start();
  }

  dispatchAsync(s_dispatch_thread, block);
}

void dispatchSyncMain(FnBlock block)
{
  qRegisterMetaType<FnBlock>("FnBlock");

  QThread *mainThread = QCoreApplication::instance()->thread();
  WorkerClass *worker = new WorkerClass(mainThread);
  QMetaObject::invokeMethod(worker,
                            "doWork",
                            Qt::BlockingQueuedConnection,
                            Q_ARG(FnBlock, block));
}

//void dispatchAsync(QPointer<QWidget> widget, FnBlock block)
//{
//  dispatchAsync([widget, block] {
//    if (!widget)
//      return;
//    block();
//  });
//}

//void dispatchAsync(QPointer<QWidget> widget, QThread* thread, FnBlock block)
//{
//  dispatchAsync(thread, [widget, block] {
//    if (!widget)
//      return;
//    block();
//  });
//}

//void dispatchAsyncMain(QPointer<QWidget> widget, FnBlock block)
//{
//  dispatchAsyncMain([widget, block] {
//    if (!widget)
//      return;
//    block();
//  });
//}

//void dispatchSyncMain(QPointer<QWidget> widget, FnBlock block)
//{
//  dispatchSyncMain([widget, block] {
//    if (!widget)
//      return;
//    block();
//  });
//}


void dispatchAsync(QPointer<QObject> widget, FnBlock block)
{
  dispatchAsync([widget, block] {
    if (!widget)
      return;
    block();
  });
}

void dispatchAsync(QPointer<QObject> widget, QThread* thread, FnBlock block)
{
  dispatchAsync(thread, [widget, block] {
    if (!widget)
      return;
    block();
  });
}

void dispatchAsyncMain(QPointer<QObject> widget, FnBlock block)
{
  dispatchAsyncMain([widget, block] {
    if (!widget)
      return;
    block();
  });
}

void dispatchSyncMain(QPointer<QObject> widget, FnBlock block)
{
  dispatchSyncMain([widget, block] {
    if (!widget)
      return;
    block();
  });
}

