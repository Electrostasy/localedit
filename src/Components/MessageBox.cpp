#include "MessageBox.h"

QMessageBox *MessageBox::UnsavedChangesBox(const QString windowTitle) {
	QMessageBox *unsavedChangesBox = new QMessageBox();
	unsavedChangesBox->setWindowTitle(windowTitle);
	unsavedChangesBox->setText("You have unsaved changes.");
	unsavedChangesBox->setStandardButtons(QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
	unsavedChangesBox->setDefaultButton(QMessageBox::Cancel);
	return unsavedChangesBox;
}

QMessageBox *MessageBox::OverwritingFilesBox(const QString windowTitle, const QString directory) {
	QMessageBox *overwritingFilesBox = new QMessageBox();
	overwritingFilesBox->setWindowTitle(windowTitle);
	overwritingFilesBox->setText("You are about to overwrite files in " + directory + ". Continue?");
	overwritingFilesBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	overwritingFilesBox->setDefaultButton(QMessageBox::Cancel);
	return overwritingFilesBox;
}
