/*
* Copyright (C) 2008-2013 Communi authors
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
*/

#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* document) : QSyntaxHighlighter(document)
{
    d.color = QColor("#808080"); // TODO
}

QColor SyntaxHighlighter::highlightColor() const
{
    return d.color;
}

void SyntaxHighlighter::setHighlightColor(const QColor& color)
{
    d.color = color;
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
    if (currentBlockState() > 0) {
        // gray-out everything except links
        QTextBlock block = currentBlock();
        for (QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it) {
            QTextFragment fragment = it.fragment();
            if (fragment.isValid() && !fragment.charFormat().isAnchor())
                setFormat(fragment.position() - block.position(), fragment.length(), d.color);
        }
    }
}