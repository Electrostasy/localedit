#include <QtGui>
#include <QWidget>

#include "MainWindow.h"

/* KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK Title bar
 * ----------+-------------------------
 * PPPPPPP***|ddddddddddddddddddddddddd Search, filters     Mockup
 * UUUUUUUUUU|ddddddddddddddddddddddddd Mission list        Mockup
 * UUUUUUUUUU|ddddddddddddddddddddddddd Mission list        Mockup
 * UUUUUUUUUU|qqqqqqqqqqqqqqqqqqqqqqqqq Mission list        Writing settings
 * UUUUUUUUUU|rrrrrrrrrrrrrrrrrrrrrrrrr Mission list        Writing field
 * UUUUUUUUUU|rrrrrrrrrrrrrrrrrrrrrrrrr Mission list        Writing field
 * UUUUUUUUUU|rrrrrrrrrrrrrrrrrrrrrrrrr Mission list        Writing field
 * UUUUUUUUUU|rrrrrrrrrrrrrrrrrrrrrrrrr Mission list        Writing field
 */

MainWindow::MainWindow(QWidget *parent) {
    // Stack all widgets (layouts) horizontally
    auto masterHorizontalLayout = new QHBoxLayout(this);

    { // Construct vertical column for mission selection
        // Create a label and search field
        auto *label = new QLabel("Missions");
        auto missionSearch = new QLineEdit();
        missionSearch->setPlaceholderText("Search missions...");

        // Create a list to search through
        missionList = new QListWidget();

        // Connect search to list
        connect(missionSearch, &QLineEdit::textChanged, this, [=](const QString& filter) {
            for(int i = 0; i < missionList->count(); ++i) {
                auto item = missionList->item(i);

                // Show item if match, otherwise hide
                item->setHidden(!item->text().contains(filter, Qt::CaseInsensitive));
            }
        });

        // Add widgets to a vertical layout
        auto *missionsBox = new QVBoxLayout();
        missionsBox->addWidget(label);
        missionsBox->addWidget(missionSearch);
        missionsBox->addWidget(missionList);

        // Add the vertical layout for missions to the main window
        masterHorizontalLayout->addLayout(missionsBox);
    }
}

void MainWindow::addMissionToList(const QString &missionCode, const QString &missionName) {
    auto *item = new QListWidgetItem(missionList);
	item->setData(0, missionCode);
    item->setText(missionName);
}

// Accept for drag and drop only physical (local) files that have a path on disk
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

// Drop multiple files onto the window and load them from path as read only
void MainWindow::dropEvent(QDropEvent *event) {
	QRegularExpression rxMissionCodeTemplates("(?<=MissionTemplates_).+?(?=_MissionTitle)");
	QRegularExpression rxMissionCodeObjectives("(?<=TaskObjectives_).+?(?=_Stage)");
	QRegularExpression rxRemainder("(?<=\\=).*");

	foreach (const QUrl &url, event->mimeData()->urls()) {
		QFile file(url.toLocalFile());
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return;
		}

		QTextStream namesStream(&file);
		while (!namesStream.atEnd()) {
			QString line = namesStream.readLine();

			// If file is MissionTemplates (mission code, mission name)
			if (line.startsWith("MissionTemplates_")) {
				QRegularExpressionMatch matchMissionCode = rxMissionCodeTemplates.match(line);
				if (matchMissionCode.hasMatch()) {
					QRegularExpressionMatch matchMissionName = rxRemainder.match(line);
					if (matchMissionName.hasMatch()) {
						missionMap[matchMissionCode.captured(0)] = matchMissionName.captured(0);
						addMissionToList(matchMissionCode.captured(0), matchMissionName.captured(0));
					}
				}
			}

			// If file is TaskObjectives (mission code, mission stage and description)
			if (line.startsWith("TaskObjectives_")) {
				QRegularExpressionMatch matchMissionCode = rxMissionCodeObjectives.match(line);
				if (matchMissionCode.hasMatch()) {
					QRegularExpressionMatch matchMissionText = rxRemainder.match(line);
					if (matchMissionText.hasMatch()) {
						if (line.contains("_OwnerBrief=")) {
							missionStagesMap[matchMissionCode.captured(0)].first.push_back(matchMissionText.captured(0));
						}

						if (line.contains("_DispatchBrief=")) {
							missionStagesMap[matchMissionCode.captured(0)].second.push_back(matchMissionText.captured(0));
						}
					}
				}
			}
		}

		file.close();
	}
}
