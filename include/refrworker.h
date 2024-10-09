#ifndef REFRWORKER_H
#define REFRWORKER_H
#include <QObject>
#include <QSplineSeries>
#include <QScatter3DSeries>
#include "refrlogic1.h"

class RefrWorker : public QObject {
	Q_OBJECT
public:
	RefrWorker();

	struct SeriesData {
		QScatter3DSeries	 *scatter3d;
		std::vector<QPointF> dataPlot[4];
	};

	void		process();

	SeriesData  *getData();

	RefrLogic1	getRefrData() const;
	void		setValues(const RefrLogicData &values);
signals:
	void finished();
	void progressChanged(int value);
private:
	std::vector<QPointF> plot2Dfunc_refr(const std::vector<double> &z_array);
private:
	RefrLogic1 refr1;
	SeriesData data;
};

#endif // REFRWORKER_H
