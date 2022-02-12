/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QClipboard>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVBoxLayout>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mLoadingLabel(new QLabel(tr("Loading..."))),
      mAddressLabel(new QLabel),
      mErrorLabel(new QLabel),
      mRefreshButton(new QPushButton(tr("Refresh"))),
      mCopyButton(new QPushButton(tr("Copy")))
{
    mLoadingLabel->setAlignment(Qt::AlignCenter);

    mAddressLabel->setAlignment(Qt::AlignCenter);
    mAddressLabel->setStyleSheet("QLabel{color: #007700; font-size: 18pt; font-weight: bold;}");

    mErrorLabel->setAlignment(Qt::AlignCenter);
    mErrorLabel->setStyleSheet("QLabel{color: #770000;}");

    connect(mRefreshButton, &QPushButton::clicked, this, &MainWindow::refreshIP);

    connect(mCopyButton, &QPushButton::clicked, this, &MainWindow::copyIP);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addStretch(1);
    hboxLayout->addWidget(mRefreshButton);
    hboxLayout->addWidget(mCopyButton);
    hboxLayout->addStretch(1);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(mLoadingLabel);
    vboxLayout->addWidget(mAddressLabel);
    vboxLayout->addWidget(mErrorLabel);
    vboxLayout->addLayout(hboxLayout);

    QWidget *widget = new QWidget;
    widget->setLayout(vboxLayout);
    setCentralWidget(widget);

    resize(480, 200);

    setWindowTitle(tr("IP Helper"));
    setWindowIcon(QIcon(":/iphelper.png"));

    refreshIP();
}

void MainWindow::refreshIP()
{
    mLoadingLabel->show();
    mAddressLabel->hide();
    mErrorLabel->hide();
    mRefreshButton->setEnabled(false);
    mCopyButton->setEnabled(false);

    QNetworkRequest request(QUrl("https://api.ipify.org"));
    QNetworkReply *reply = mNetworkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this, &MainWindow::onReplyFinished);
}

void MainWindow::copyIP()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(mText);
}

void MainWindow::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        mText = QString::fromUtf8(reply->readAll());
        mAddressLabel->setText(mText);
        mAddressLabel->show();
    } else {
        mText = reply->errorString();
        mErrorLabel->setText(mText);
        mErrorLabel->show();
    }

    mLoadingLabel->hide();
    mRefreshButton->setEnabled(true);
    mCopyButton->setEnabled(true);
}
