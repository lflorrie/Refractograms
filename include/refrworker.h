#ifndef REFRWORKER_H
#define REFRWORKER_H
#include <QObject>
#include <QSplineSeries>
#include <QScatter3DSeries>
#include "refrlogic1.h"
#include "refrtypes.h"

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

	void setTypeOfCalculation(Plots p);
signals:
	void finished(Plots type);
	void progressChanged(int value);
private:
	std::vector<QPointF> plot2Dfunc_refr(const std::vector<double> &z_array);
	void calculateAll();
	void calculate3_1();
private:
	RefrLogic1 refr1;
	SeriesData data;
	Plots	   typeOfCalculation;

};

#endif // REFRWORKER_H
