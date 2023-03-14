/***************************************************************************
 *   Copyright (C) 2012 by Joerg Zopes                                     *
 *   joerg.zopes@linux-specialist.com                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "highlighter.h"
//
highlighter::highlighter(QTextDocument * parent) : QSyntaxHighlighter(parent)
{
     HighlightingRule rule;

     keywordFormat.setForeground(Qt::darkBlue);
     keywordFormat.setFontWeight(QFont::Bold);

     singleLineCommentFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("^Downloading.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("^/.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp(".Empty file");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("^WARN.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp(".FOUND *");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp(".ERROR:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp(".WARNING:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("^Can't connect to port.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::green);
     rule.pattern = QRegExp("^daily.cvd.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("^Database updated.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::green);
     rule.pattern = QRegExp("^bytecode.cvd.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::green);
     rule.pattern = QRegExp("^main.cvd.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::blue);
     rule.pattern = QRegExp("^ClamAV update process started.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::green);
     rule.pattern = QRegExp("^freshclam deamon.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::green);
     rule.pattern = QRegExp(". OK");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

          singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp("^---.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(Qt::red);
     rule.pattern = QRegExp(".Access denied");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#0000ff"));
     rule.pattern = QRegExp("^Known viruses:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#0000ff"));
     rule.pattern = QRegExp("^Engine version:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#000000"));
     rule.pattern = QRegExp("^Scanned directories:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#000000"));
     rule.pattern = QRegExp("^Scanned files:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#ff0000"));
     rule.pattern = QRegExp("^Infected files:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#ff0000"));
     rule.pattern = QRegExp("^Total errors:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#0000ff"));
     rule.pattern = QRegExp("^Data scanned:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#0000ff"));
     rule.pattern = QRegExp("^Data read:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     singleLineCommentFormat.setForeground(QColor("#0000ff"));
     rule.pattern = QRegExp("^Time:.*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);

     multiLineCommentFormat.setForeground(Qt::red);
     commentStartExpression = QRegExp("START");
     commentEndExpression = QRegExp("ENDE");

}

void highlighter::highlightBlock(const QString &text)
 {
     foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
     }
     
     setCurrentBlockState(0);

     int startIndex = 0;
     if (previousBlockState() != 1) startIndex = commentStartExpression.indexIn(text);
     
     while (startIndex >= 0) {
         int endIndex = commentEndExpression.indexIn(text, startIndex);
         int commentLength;
         if (endIndex == -1) {
             setCurrentBlockState(1);
             commentLength = text.length() - startIndex;
         } else {
             commentLength = endIndex - startIndex
                             + commentEndExpression.matchedLength();
         }
         startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
//
