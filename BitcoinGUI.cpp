/*
 * W.J. van der Laan 2011
 */
#include "BitcoinGUI.h"
#include "TransactionTableModel.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QTabBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QLocale>

#include <iostream>

BitcoinGUI::BitcoinGUI(QWidget *parent):
    QMainWindow(parent)
{
    resize(850, 550);
    setWindowTitle(tr("Bitcoin"));
    setWindowIcon(QIcon(":icons/bitcoin"));
    
    QAction *quit = new QAction(QIcon(":/icons/quit"), "&Quit", this);
    QAction *sendcoins = new QAction(QIcon(":/icons/send"), "&Send coins", this);
    QAction *addressbook = new QAction(QIcon(":/icons/address-book"), "&Address book", this);
    QAction *about = new QAction(QIcon(":/icons/bitcoin"), "&About", this);
    
    /* Menus */
    QMenu *file = menuBar()->addMenu("&File");
    file->addAction(sendcoins);
    file->addSeparator();
    file->addAction(quit);
    
    QMenu *settings = menuBar()->addMenu("&Settings");
    settings->addAction(addressbook);

    QMenu *help = menuBar()->addMenu("&Help");
    help->addAction(about);
    
    /* Toolbar */
    QToolBar *toolbar = addToolBar("Main toolbar");
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addAction(sendcoins);
    toolbar->addAction(addressbook);

    /* Address: <address>: New... : Paste to clipboard */
    QHBoxLayout *hbox_address = new QHBoxLayout();
    hbox_address->addWidget(new QLabel(tr("Your Bitcoin Address:")));
    QLineEdit *edit_address = new QLineEdit();
    edit_address->setReadOnly(true);
    hbox_address->addWidget(edit_address);
    
    QPushButton *button_new = new QPushButton(trUtf8("&New\u2026"));
    QPushButton *button_clipboard = new QPushButton(tr("&Copy to clipboard"));
    hbox_address->addWidget(button_new);
    hbox_address->addWidget(button_clipboard);
    
    /* Balance: <balance> */
    QHBoxLayout *hbox_balance = new QHBoxLayout();
    hbox_balance->addWidget(new QLabel(tr("Balance:")));
    hbox_balance->addSpacing(5);/* Add some spacing between the label and the text */

    QLabel *label_balance = new QLabel(QLocale::system().toString(1345.54));
    label_balance->setFont(QFont("Teletype"));
    hbox_balance->addWidget(label_balance);
    hbox_balance->addStretch(1);
    
    /* Tab widget */
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox_address);
    vbox->addLayout(hbox_balance);
    
    /* Transaction table:
     * TransactionView
     * TransactionModel
     */
    QTableView *transaction_table = new QTableView(this);

    TransactionTableModel *transaction_model = new TransactionTableModel(this);
    transaction_table->setModel(transaction_model);
    transaction_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    transaction_table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    transaction_table->horizontalHeader()->resizeSection(
            TransactionTableModel::Status, 112);
    transaction_table->horizontalHeader()->resizeSection(
            TransactionTableModel::Date, 112);
    transaction_table->horizontalHeader()->setResizeMode(
            TransactionTableModel::Description, QHeaderView::Stretch);
    transaction_table->horizontalHeader()->resizeSection(
            TransactionTableModel::Debit, 79);
    transaction_table->horizontalHeader()->resizeSection(
            TransactionTableModel::Credit, 79);

    /* setupTabs */
    QTabBar *tabs = new QTabBar(this);
    tabs->addTab(tr("All transactions"));
    tabs->addTab(tr("Sent/Received"));
    tabs->addTab(tr("Sent"));
    tabs->addTab(tr("Received"));
    /* QSortFilterProxyModel
       setFilterRole : filter on user role
       setFilterKeyColumn
       setFilterRegExp / setFilterFixedString
       "^."
       "^[sr]"
       "^[s]"
       "^[r]"
     */
   
    vbox->addWidget(tabs);
    vbox->addWidget(transaction_table);
    
    QWidget *centralwidget = new QWidget(this);
    centralwidget->setLayout(vbox);
    setCentralWidget(centralwidget);
    
    /* Status bar */
    statusBar();
    
    QLabel *label_connections = new QLabel("6 connections", this);
    label_connections->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label_connections->setMinimumWidth(100);
    
    QLabel *label_blocks = new QLabel("6 blocks", this);
    label_blocks->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label_blocks->setMinimumWidth(100);
    
    QLabel *label_transactions = new QLabel("6 transactions", this);
    label_transactions->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label_transactions->setMinimumWidth(100);
    
    
    statusBar()->addPermanentWidget(label_connections);
    statusBar()->addPermanentWidget(label_blocks);
    statusBar()->addPermanentWidget(label_transactions);
    
     
    /* Action bindings */

    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
}

void BitcoinGUI::currentChanged(int tab)
{
    std::cout << "Switched to tab: " << tab << std::endl;
}
