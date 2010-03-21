#ifndef SPECIFICATION_H
#define SPECIFICATION_H

#include <QThread>
#include <QTimer>
#include <QImage>
#include <QMutex>

#include <QTime>

#include "graphics/image.h"

class Generator;

class Specification {
public:
	virtual Generator* createGenerator(int width, int height) const = 0;
};

class Thread : public QThread {
	const int index_;
	Generator& parent_;
public:
	Thread(int index, Generator& parent); // Obtains a ticket from parent
	void run(); // Calls execute in parent
};

class Generator : public QObject {
	Q_OBJECT

	bool isStopped_;

	int runningCount_;

	bool isSelectable_;

	int updateInterval_;

	bool needsImageRefresh_;
	QTimer updateTimer_;

	QList<Thread*> threads_;

	QMutex mutex_;
	QMutex threadMutex_;

public:
	Generator(int threadCount, int updateInterval, bool needsImageRefresh, bool isSelectable);
	~Generator();

	virtual const Specification& specification() const = 0;

	bool isRunning() const; // runningCount_ > 0?

	virtual int progress() = 0;
	virtual int totalSteps() = 0;

	bool isSelectable() const;

	// This method may trigger updates
	// in image containers
	const QImage& image() const;

	int width() const;
	int height() const;

	void setSize(int width, int height);

	qreal normX(qreal x) const;
	qreal normY(qreal y) const;

	qreal denormX(qreal x0) const;
	qreal denormY(qreal y0) const;

	virtual void scale(int cx, int cy, qreal factor) = 0;
	virtual void move(int dx, int dy) = 0;
	virtual void select(qreal wx, qreal wy, qreal hx, qreal hy, qreal x0, qreal y0) = 0;

	virtual QString pointDescription(qreal x, qreal y) = 0;

signals:
	void started();
	void done(bool cancelled);

	void updated(int progress, int totalSteps);
	void resized(int width, int height);

public slots:
	void start();
	void cancel();
	void cancelWait();

protected:
	virtual Image& img() = 0;
	virtual const Image& img() const = 0;

	virtual Specification& specification() = 0;

	/** Threads have to check this regularly and must terminate immediately
	  * if this function returns true */
	bool isStopped() const;

	/** Obtains lock, cancels tasks (if running) and waits for termination */
	void lockCancelWait();

	/** Starts threads and releases lock */
	void startUnlock();

	/** Some initialization code for subclasses */
	virtual void init() = 0;

	/** Method called by the index-th thread */
	virtual void exec(int index, int count) = 0;

protected slots:
	/** Sends the update(int progress, int totalSteps)-message */
	void emitUpdate();


private:
	void run(int i);

friend class Thread;
};

#endif // SPECIFICATION_H