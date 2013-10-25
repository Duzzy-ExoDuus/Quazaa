#include "dialogimportsecurity.h"
#include "ui_dialogimportsecurity.h"

#include "securitymanager.h"
#include "quazaasettings.h"

#include <QFileDialog>

CDialogImportSecurity::CDialogImportSecurity(QWidget *parent) :
QDialog(parent),
ui(new Ui::CDialogImportSecurity)
{
	ui->setupUi(this);
}

CDialogImportSecurity::~CDialogImportSecurity()
{
	delete ui;
}

void CDialogImportSecurity::on_toolButtonChooseFile_clicked()
{
	QString filter;
	switch (ui->comboBoxFilterType->currentIndex()) {
		case SecurityFilterType::Shareaza:
			filter = tr("Shareaza Security Filters (*.xml)");
			break;
		case SecurityFilterType::P2P:
			filter = tr("P2P Format Ban List (*.p2p *.txt)");
			break;
		case SecurityFilterType::Dat:
			filter = tr("Dat Format Ban List (*.dat *.txt)");
			break;
		default: //SecurityFilterType::Quazaa
			filter = tr("Quazaa Security Filters (*.qsf)");
			break;
	}

	QFileDialog* dlgFile = new QFileDialog(this);
	ui->lineEditFile->setText(dlgFile->getOpenFileName(this,
		tr("Select File to Import"), quazaaSettings.Downloads.CompletePath, filter));

	if(!ui->lineEditFile->text().isEmpty())
		ui->pushButtonOK->setEnabled(true);
}

void CDialogImportSecurity::on_pushButtonOK_clicked()
{
	switch (ui->comboBoxFilterType->currentIndex()) {
		case SecurityFilterType::Shareaza:
			break;
		case SecurityFilterType::P2P:
			securityManager.fromP2P(ui->lineEditFile->text());
			break;
		case SecurityFilterType::Dat:
			break;
		default: //SecurityFilterType::Quazaa
			break;
	}

	close();
}

void CDialogImportSecurity::on_pushButtonCancel_clicked()
{
	close();
}

void CDialogImportSecurity::on_comboBoxFilterType_currentIndexChanged(int index)
{
	Q_UNUSED(index);

	ui->lineEditFile->text().clear();
	ui->pushButtonOK->setEnabled(false);
}
