/*
 * Copyright 2002-2004  Kevin Tambascio <ktambascio@users.sourceforge.net>
 * Copyright 2002-2019  Thomas Baumgart <tbaumgart@kde.org>
 * Copyright 2004-2005  Ace Jones <acejones@users.sourceforge.net>
 * Copyright 2006       Darren Gould <darren_gould@gmx.de>
 * Copyright 2017-2018  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MYMONEYSTORAGEXML_H
#define MYMONEYSTORAGEXML_H

#include <memory>

// ----------------------------------------------------------------------------
// QT Includes

#include <QMap>
#include <QString>
#include <QPointer>

// ----------------------------------------------------------------------------
// Project Includes

#include "imymoneystorageformat.h"

/**
  *@author Kevin Tambascio (ktambascio@users.sourceforge.net)
  */

#define VERSION_0_60_XML  0x10000010    // Version 0.5 file version info
#define VERSION_0_61_XML  0x10000011    // use 8 bytes for MyMoneyMoney objects

class QString;
class QIODevice;
class QDomElement;
class QDomDocument;
class QDate;

class MyMoneyInstitution;
class MyMoneyAccount;
class MyMoneySecurity;
class MyMoneySchedule;
class MyMoneyPayee;
class MyMoneyTag;
class MyMoneyBudget;
class MyMoneyReport;
class MyMoneyPrice;
class MyMoneyTransaction;
class MyMoneyCostCenter;
class onlineJob;
class MyMoneyFile;
class PriceEntry;

template <typename T> class QList;
typedef QPair<QString, QString> MyMoneySecurityPair;
typedef QMap<QDate, MyMoneyPrice> MyMoneyPriceEntries;
typedef QMap<MyMoneySecurityPair, MyMoneyPriceEntries> MyMoneyPriceList;

class ReaderWriter
{
  friend class MyMoneyXmlContentHandler;
public:
  ReaderWriter();
  virtual ~ReaderWriter();

  enum fileVersionDirectionType {
    Reading = 0,          /**< version of file to be read */
    Writing = 1           /**< version to be used when writing a file */
  };

  void readFile(const QByteArray &rawStorage, MyMoneyFile &file);
  QByteArray writeFile(const MyMoneyFile &file);
  void setProgressCallback(void(*callback)(int, int, const QString&));

  protected:
  void          signalProgress(int current, int total, const QString& = "");

  /**
    * This method returns the version of the underlying file. It
    * is used by the MyMoney objects contained in a MyMoneyStorageBin object (e.g.
    * MyMoneyAccount, MyMoneyInstitution, MyMoneyTransaction, etc.) to
    * determine the layout used when reading/writing a persistent file.
    * A parameter is used to determine the direction.
    *
    * @param dir information about the direction (reading/writing). The
    *            default is reading.
    *
    * @return version QString of file's version
    *
    * @see m_fileVersionRead, m_fileVersionWrite
    */
  static unsigned int fileVersion(fileVersionDirectionType dir = Reading);

  QList<QDomElement> readElements(QString groupTag, QString itemTag = QString());

  bool readFileInformation(const QDomElement& fileInfo);
  virtual void writeFileInformation(QDomElement& fileInfo);

  virtual void writeUserInformation(QDomElement& userInfo);

  virtual void writeInstitution(QDomElement& institutions, const MyMoneyInstitution& i);
  virtual void writeInstitutions(QDomElement& institutions);

  virtual void writePrices(QDomElement& prices);
  /// @todo cleanup
  // virtual void writePricePair(QDomElement& price, const MyMoneyPriceEntries& p);
  // virtual void writePrice(QDomElement& prices, const MyMoneyPrice& p);

  virtual void writePayees(QDomElement& payees);
  virtual void writePayee(QDomElement& payees, const MyMoneyPayee& p);

  virtual void writeTags(QDomElement& tags);
  virtual void writeTag(QDomElement& tags, const MyMoneyTag& ta);

  virtual void writeAccounts(QDomElement& accounts);
  virtual void writeAccount(QDomElement& accounts, const MyMoneyAccount& p);

  virtual void writeTransactions(QDomElement& transactions);
  virtual void writeTransaction(QDomElement& transactions, const MyMoneyTransaction& tx);

  virtual void writeSchedules(QDomElement& scheduled);
  virtual void writeSchedule(QDomElement& scheduledTx, const MyMoneySchedule& tx);

  virtual void writeReports(QDomElement& e);
  virtual void writeReport(QDomElement& report, const MyMoneyReport& r);
  virtual void writeBudgets(QDomElement& e);
  virtual void writeBudget(QDomElement& budget, const MyMoneyBudget& b);

  virtual void writeOnlineJobs(QDomElement& onlineJobs);
  virtual void writeOnlineJob(QDomElement& onlineJobs, const onlineJob& job);

  virtual void writeSecurities(QDomElement& securities);
  virtual void writeSecurity(QDomElement& securityElement, const MyMoneySecurity& security);

  virtual void writeCostCenters(QDomElement& parent);
  virtual void writeCostCenter(QDomElement& costCenters, const MyMoneyCostCenter& costCenter);

  virtual void writeCurrencies(QDomElement& currencies);

  virtual QDomElement writeKeyValuePairs(const QMap<QString, QString> pairs);

  bool readUserInformation(const QDomElement& userElement);

  void readPricePair(const QDomElement& pricePair);
  const MyMoneyPrice readPrice(const QString& from, const QString& to, const QDomElement& price);

  QDomElement findChildElement(const QString& name, const QDomElement& root);

private:
  void (*m_progressCallback)(int, int, const QString&);

protected:
  MyMoneyFile*        m_file;
  QDomDocument*       m_doc;

private:
  /// \internal d-pointer class.
  class Private;
  /// \internal d-pointer instance.
  Private* const d;

  /**
    * This member is used to store the file version information
    * obtained while reading a file.
    */
  static unsigned int fileVersionRead;

  /**
    * This member is used to store the file version information
    * to be used when writing a file.
    */
  static unsigned int fileVersionWrite;
  /**
    * This member keeps the id of the base currency. We need this
    * temporarily to convert the price history from the old to the
    * new format. This should go at some time beyond 0.8 (ipwizard)
    */
  QString m_baseCurrencyId;

};

#endif
