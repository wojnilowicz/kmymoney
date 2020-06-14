/*
 * Copyright 2020  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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

#include "kcm.h"
#include "cmakedefine.h"

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

#include <KPluginFactory>

// ----------------------------------------------------------------------------
// Project Includes

#include "kmymoneyplugin.h"

#include "configuration.h"
#include "storage/interface/ui.h"
#include "storage/interface/enums.h"
#include "storage/interface/iplugin.h"
#include "ui_kcm.h"

namespace Storage {
namespace Manager {

KCM::KCM(QWidget *parent, const QVariantList& args) :
  KCModule(parent, args),
  ui(std::make_unique<Ui::KCM>())
{
  ui->setupUi(this);

#ifndef ENABLE_QUICK
  ui->rbQuick->setEnabled(false);
#endif

  auto uiType = StorageUi::typeEnum(Configuration::self()->uiType());
  switch (uiType) {
    case eUI::Quick:
      if (ui->rbQuick->isEnabled())
        ui->rbQuick->setChecked(true);
      break;
    case eUI::Widget:
    default:
      ui->rbWidget->setChecked(true);
  }

  for (const auto& plugin : pPlugins.newstorage) {
    for (const auto &type : plugin->types()) {
      if (plugin->canOpen(type) || plugin->canSave(type))
        availableTypesMap.insert(plugin->typeInternalName(type), plugin->typeDisplayedName(type));
    }
  }

  QStringList availableTypesList = availableTypesMap.keys();
  QStringList visibleTypesList = Configuration::self()->visibleStorageTypes();

  ui->hiddenTypesList->addItems(availableTypesMap.values());
  ui->visibleTypesList->addItems(availableTypesMap.values());
  for (auto i = 0 ; i < ui->visibleTypesList->count(); ++i) {
    auto currentType = availableTypesList.at(i);
    auto visible = visibleTypesList.contains(currentType);
    ui->visibleTypesList->item(i)->setHidden(!visible);
    ui->hiddenTypesList->item(i)->setHidden(visible);
  }

  auto addButtonClicked = [=]() {
    for (auto& hiddenType : ui->hiddenTypesList->selectedItems()) {
      hiddenType->setHidden(true);
      auto row = ui->hiddenTypesList->row(hiddenType);
      ui->visibleTypesList->item(row)->setHidden(false);
    }
    ui->addButton->setEnabled(false);
  };

  auto removeButtonClicked = [=]() {
    for (auto& visibleType : ui->visibleTypesList->selectedItems()) {
      visibleType->setHidden(true);
      auto row = ui->visibleTypesList->row(visibleType);
      ui->hiddenTypesList->item(row)->setHidden(false);
    }
    ui->removeButton->setEnabled(false);
  };

  auto hiddenTypesListActivated = [=]() {
    ui->visibleTypesList->selectionModel()->clear();
  };

  auto hiddenTypesListSelectionChanged = [=]() {
    ui->addButton->setEnabled(ui->hiddenTypesList->selectedItems().count() > 0);
  };

  auto visibleTypesListActivated = [=]() {
    ui->hiddenTypesList->selectionModel()->clear();
  };

  auto visibleTypesListSelectionChanged = [=]() {
    ui->removeButton->setEnabled(ui->visibleTypesList->selectedItems().count() > 0);
  };

  connect(ui->addButton, &QPushButton::clicked, addButtonClicked);
  connect(ui->removeButton, &QPushButton::clicked, removeButtonClicked);

  connect(ui->hiddenTypesList, &QListWidget::itemClicked, hiddenTypesListActivated);
  connect(ui->hiddenTypesList, &QListWidget::itemSelectionChanged, hiddenTypesListSelectionChanged);

  connect(ui->visibleTypesList, &QListWidget::itemClicked, visibleTypesListActivated);
  connect(ui->visibleTypesList, &QListWidget::itemSelectionChanged, visibleTypesListSelectionChanged);
}

KCM::~KCM()
{
  QString uiTypeString;
  if (ui->rbQuick->isChecked())
    uiTypeString = StorageUi::typeString(eUI::Quick);
  else if (ui->rbWidget->isChecked())
    uiTypeString = StorageUi::typeString(eUI::Widget);

  if (!uiTypeString.isEmpty())
    Configuration::self()->setUiType(uiTypeString);

  QStringList availableTypesList = availableTypesMap.keys();
  QStringList visibleTypesList;
  for (auto i = 0 ; i < ui->visibleTypesList->count(); ++i) {
    if (!ui->visibleTypesList->item(i)->isHidden())
      visibleTypesList.append(availableTypesList.at(i));
  }
  Configuration::self()->setVisibleStorageTypes(visibleTypesList);
  Configuration::self()->save();

  delete Configuration::self();
}

K_PLUGIN_FACTORY_WITH_JSON(KCMFactory, "kcm.json", registerPlugin<KCM>();)

} // namespace Manager
} // namespace Storage

#include "kcm.moc"
