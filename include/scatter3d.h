#ifndef SCATTER3D_H
#define SCATTER3D_H
#include <Q3DScatter>

class Scatter3D : public Q3DScatter
{
	Q_OBJECT
public:
	Scatter3D();
	void saveContent(const QString &full_path);
	void exportContent(const QString &full_path);
protected:
	virtual void mousePressEvent(QMouseEvent *) override;
};

#endif // SCATTER3D_H
