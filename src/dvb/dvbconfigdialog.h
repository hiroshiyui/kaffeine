/*
 * dvbconfigdialog.h
 *
 * Copyright (C) 2007-2009 Christoph Pfister <christophpfister@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef DVBCONFIGDIALOG_H
#define DVBCONFIGDIALOG_H

#include <KDialog>

class QBoxLayout;
class QGridLayout;
class QLabel;
class QProgressBar;
class QSpinBox;
class QTreeWidget;
class KComboBox;
class KJob;
class KLineEdit;
namespace KIO
{
class Job;
class TransferJob;
}
class DvbConfig;
class DvbConfigBase;
class DvbConfigPage;
class DvbDeviceConfig;
class DvbManager;
class DvbSConfigObject;

class DvbConfigDialog : public KDialog
{
	Q_OBJECT
public:
	DvbConfigDialog(DvbManager *manager_, QWidget *parent);
	~DvbConfigDialog();

private slots:
	void changeRecordingFolder();
	void changeTimeShiftFolder();
	void updateScanFile();
	void latitudeChanged(const QString &text);
	void longitudeChanged(const QString &text);

private:
	static double toLatitude(const QString &text, bool *ok);
	static double toLongitude(const QString &text, bool *ok);

	void accept();

	DvbManager *manager;
	KLineEdit *recordingFolderEdit;
	KLineEdit *timeShiftFolderEdit;
	KLineEdit *latitudeEdit;
	KLineEdit *longitudeEdit;
	QPixmap validPixmap;
	QPixmap invalidPixmap;
	QLabel *latitudeValidLabel;
	QLabel *longitudeValidLabel;
	QList<DvbConfigPage *> configPages;
};

class DvbScanFileDownloadDialog : public KDialog
{
	Q_OBJECT
public:
	DvbScanFileDownloadDialog(DvbManager *manager_, QWidget *parent);
	~DvbScanFileDownloadDialog();

private slots:
	void progressChanged(KJob *, unsigned long percent);
	void dataArrived(KIO::Job *, const QByteArray &data);
	void jobFinished();

private:
	DvbManager *manager;
	QProgressBar *progressBar;
	QLabel *label;
	KIO::TransferJob *job;
	QByteArray scanData;
};

class DvbConfigPage : public QWidget
{
public:
	DvbConfigPage(QWidget *parent, DvbManager *manager, const DvbDeviceConfig &deviceConfig);
	~DvbConfigPage();

	QList<DvbConfig> getConfigs();

private:
	void addHSeparator(const QString &title);

	QBoxLayout *boxLayout;
	QList<DvbConfig> configs;
	DvbSConfigObject *dvbSObject;
};

class DvbConfigObject : public QObject
{
	Q_OBJECT
public:
	DvbConfigObject(QWidget *parent, QBoxLayout *layout, DvbManager *manager,
		DvbConfigBase *config_);
	~DvbConfigObject();

private slots:
	void timeoutChanged(int timeout);
	void sourceChanged(int index);
	void nameChanged();

private:
	DvbConfigBase *config;
	QString defaultName;
	KComboBox *sourceBox;
	KLineEdit *nameEdit;
};

class DvbSConfigObject : public QObject
{
	Q_OBJECT
public:
	DvbSConfigObject(QWidget *parent_, QBoxLayout *boxLayout, DvbManager *manager,
		const QList<DvbConfig> &configs);
	~DvbSConfigObject();

	void appendConfigs(QList<DvbConfig> &list);

signals:
	void setDiseqcVisible(bool visible);
	void setRotorVisible(bool visible); // common parts of usals / positions ui
	void setUsalsVisible(bool visible); // usals-specific parts of ui
	void setPositionsVisible(bool visible); // positions-specific parts of ui

private slots:
	void configChanged(int index);
	void addSatellite();
	void removeSatellite();

private:
	DvbConfigBase *createConfig(int lnbNumber);

	QWidget *parent;
	DvbConfigBase *lnbConfig;
	QList<DvbConfig> diseqcConfigs;
	QStringList sources;
	QGridLayout *layout;
	KComboBox *configBox;
	KComboBox *sourceBox;
	QSpinBox *rotorSpinBox;
	QTreeWidget *satelliteView;
};

class DvbSLnbConfigObject : public QObject
{
	Q_OBJECT
public:
	DvbSLnbConfigObject(QSpinBox *timeoutSpinBox, KComboBox *sourceBox_,
		QPushButton *configureButton_, DvbConfigBase *config_);
	~DvbSLnbConfigObject();

private slots:
	void timeoutChanged(int value);
	void sourceChanged(int index);
	void configure();
	void selectType(int type);
	void dialogAccepted();

private:
	KComboBox *sourceBox;
	QPushButton *configureButton;
	DvbConfigBase *config;

	QLabel *lowBandLabel;
	QLabel *switchLabel;
	QLabel *highBandLabel;
	QSpinBox *lowBandSpinBox;
	QSpinBox *switchSpinBox;
	QSpinBox *highBandSpinBox;
	int currentType;
};

#endif /* DVBCONFIGDIALOG_H */
