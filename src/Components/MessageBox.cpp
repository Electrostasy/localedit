#include "MessageBox.h"

QMessageBox MessageBox::UnsavedChangesBox(std::string windowTitle) {
	QMessageBox unsavedChangesBox;
	unsavedChangesBox.setWindowTitle(windowTitle);
	unsavedChangesBox.setText("You have unsaved changes.");
	unsavedChangesBox.setStandardButtons(QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
	unsavedChangesBox.setDefaultButton(QMessageBox::Cancel);
	return unsavedChangesBox;
}
