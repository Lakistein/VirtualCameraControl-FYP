#include "VCCPane.h"

#include <QtCore/QObject>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>

#include "dzapp.h"
#include "dzscene.h"
#include <dznodebtn.h>
#include "dzline3.h"
#include <dzbasiccamera.h>
#include <string.h>
#include <sstream>
#include <ctime>
#include "ParticleSwarmOptimization.h"
#include "GeneticAlgorithm.h"
#include <dzorientedbox3.h>
#include <dznodebtn.h>

static const int c_minWidth = 200;
static const int c_minHeight = 150;

VCCPane::VCCPane() : DzPane("Virtual Camera Control")
{
	cc = new CameraControl();

	int margin = style()->pixelMetric(DZ_PM_GeneralMargin);

	QVBoxLayout *mainLyt = new QVBoxLayout();
	mainLyt->setMargin(margin);
	mainLyt->setSpacing(margin);

	nodeBtn = new DzNodeBtn(this);

	m_output = new QTextBrowser();
	m_output->setObjectName("SceneInfoTxtBrwsr");
	m_output->setMinimumSize(c_minWidth, c_minHeight);
	mainLyt->addWidget(m_output);

	mainLyt->addWidget(nodeBtn);

	dropbtn = new QComboBox(this);
	dropbtn->addItem("Extreme Close-Up");
	dropbtn->addItem("Face Close-Up");
	dropbtn->addItem("Mid Body Shot");
	dropbtn->addItem("Full Body Shot");
	dropbtn->addItem("Bird's Eye View");
	dropbtn->addItem("Worm's Eye View");
	dropbtn->addItem("High Angle");
	mainLyt->addWidget(dropbtn);

	algBtn = new QComboBox(this);
	algBtn->addItem("PSO");
	algBtn->addItem("GA");
	mainLyt->addWidget(algBtn);

	QPushButton *btnGenerate = new QPushButton("Generate Camera", this, "Test Name");
	mainLyt->addWidget(btnGenerate);

	setLayout(mainLyt);

	connect(btnGenerate, SIGNAL(pressed()), this, SLOT(GenerateCamera()));

	setMinimumSize(c_minWidth, c_minHeight);
}

VCCPane::~VCCPane()
{
}
//..\..\..\..\..\exec\$(Configuration)\$(Platform)\plugins\

void VCCPane::GenerateCamera()
{
	m_output->clear();
	int index = dropbtn->currentIndex();
	cc = new CameraControl(dropbtn->text(dropbtn->currentIndex()));


	if (nodeBtn->getNode() == NULL) 
	{
		m_output->append("Node Not Selected");
		return;
	}
	
	unsigned int start = clock();

	cc->GetShot(dzScene, (CameraControl::ShotType)index, nodeBtn->getNode(), dropbtn->text(dropbtn->currentIndex()), (CameraControl::Algorithm)algBtn->currentIndex());

	unsigned int a = clock() - start;

	m_output->append(QString::number(a));
}