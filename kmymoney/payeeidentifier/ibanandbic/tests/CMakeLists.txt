
set( payeeidentifierIbanBicTest_SCRS
   internationalaccountidentifiertest.cpp
)

kde4_add_unit_test( payeeidentifier_iban_bic-test
  TESTNAME kmymoney-payeeidentifier_iban_bic
  ${payeeidentifierIbanBicTest_SCRS}
)

target_link_libraries( payeeidentifier_iban_bic-test
  ${QT_QTTEST_LIBRARY}
  payeeidentifier_iban_bic
)

set( payeeidentifierIbanWidgetTest_SCRS
   payeeidentifierwidgettest.cpp
)

kde4_add_unit_test( payeeidentifier_iban_widget-test
  TESTNAME kmymoney-payeeidentifier_iban_widget
  ${payeeidentifierIbanWidgetTest_SCRS}
)

target_link_libraries( payeeidentifier_iban_widget-test
  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
  payeeidentifier_iban_bic_widgets
)
