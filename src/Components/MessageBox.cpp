#include "MessageBox.h"

QMessageBox *MessageBox::UnsavedChangesBox(const QString windowTitle) {
	QMessageBox *unsavedChangesBox = new QMessageBox();
	unsavedChangesBox->setWindowTitle(windowTitle);
	unsavedChangesBox->setText("You have unsaved changes.");
	unsavedChangesBox->setStandardButtons(QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
	unsavedChangesBox->setDefaultButton(QMessageBox::Cancel);
	return unsavedChangesBox;
}
