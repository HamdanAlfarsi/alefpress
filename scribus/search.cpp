#include "search.h"
#include "search.moc"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>
#include "mspinbox.h"
#include "scribusdoc.h"
#include "fontcombo.h"
#include "page.h"
#include "styleselect.h"
#include "shadebutton.h"
extern QPixmap loadIcon(QString nam);


SearchReplace::SearchReplace( QWidget* parent, ScribusDoc *doc, preV *Prefs, PageItem* ite, bool mode )
							: QDialog( parent, "SearchReplace", true, 0 )

{
	setCaption( tr( "Search/Replace" ) );
	setIcon(loadIcon("AppIcon.png"));
	QPixmap pm;
	pm = QPixmap(30, 15);
	CListe::Iterator it;
	Item = ite;
	Doc = doc;
	NotFound = false;
	SMode = mode;
	SearchReplaceLayout = new QVBoxLayout( this, 10, 5, "SearchReplaceLayout"); 
	if (mode)
	{
		SelLayout = new QHBoxLayout( 0, 0, 6, "SelLayout"); 
		Search = new QGroupBox( this, "Search" );
		Search->setTitle( tr( "Search for:" ) );
		Search->setColumnLayout(0, Qt::Vertical );
		Search->layout()->setSpacing( 2 );
		Search->layout()->setMargin( 5 );
		SearchLayout = new QGridLayout( Search->layout() );
		SearchLayout->setAlignment( Qt::AlignTop );
		SText = new QCheckBox( Search, "SText" );
		SText->setText( tr( "Text" ) );
		SearchLayout->addWidget( SText, 0, 0 );
		SStyle = new QCheckBox( Search, "SStyle" );
		SStyle->setText( tr( "Paragraph Style" ) );
		SearchLayout->addWidget( SStyle, 1, 0 );
		SFont = new QCheckBox( Search, "SFont" );
		SFont->setText( tr( "Font" ) );
		SearchLayout->addWidget( SFont, 2, 0 );
		SSize = new QCheckBox( Search, "SSize" );
		SSize->setText( tr( "Font Size" ) );
		SearchLayout->addWidget( SSize, 3, 0 );
		SEffect = new QCheckBox( Search, "SEffect" );
		SEffect->setText( tr( "Font Effects" ) );
		SearchLayout->addWidget( SEffect, 4, 0 );
		SFill = new QCheckBox( Search, "SFill" );
		SFill->setText( tr( "Fill Color" ) );
		SearchLayout->addWidget( SFill, 5, 0 );
		SFillS = new QCheckBox( Search, "SFillS" );
		SFillS->setText( tr( "Fill Shade" ) );
		SearchLayout->addWidget( SFillS, 6, 0 );
		SStroke = new QCheckBox( Search, "SStroke" );
		SStroke->setText( tr( "Stroke Color" ) );
		SearchLayout->addWidget( SStroke, 7, 0 );
		SStrokeS = new QCheckBox( Search, "SStrokeS" );
		SStrokeS->setText( tr( "Stroke Shade" ) );
		SearchLayout->addWidget( SStrokeS, 8, 0 );
		STextVal = new QLineEdit( Search, "STextVal" );
		STextVal->setEnabled(false);
		SearchLayout->addWidget( STextVal, 0, 1 );
		SStyleVal = new QComboBox( true, Search, "SStyleVal" );
		SStyleVal->setEditable(false);
		QString tmp_sty[] = { tr("Left"), tr("Center"), tr("Right"), tr("Block"), tr("Forced")};
		size_t ar_sty = sizeof(tmp_sty) / sizeof(*tmp_sty);
		for (uint a = 0; a < ar_sty; ++a)
			SStyleVal->insertItem(tmp_sty[a]);
		if (doc->Vorlagen.count() > 5)
		{
			for (uint x = 5; x < doc->Vorlagen.count(); ++x)
				SStyleVal->insertItem(doc->Vorlagen[x].Vname);
		}
		SStyleVal->listBox()->setMinimumWidth(SStyleVal->listBox()->maxItemWidth()+24);
		SStyleVal->setCurrentItem(doc->CurrentABStil);
		SStyleVal->setEnabled(false);
		SearchLayout->addWidget( SStyleVal, 1, 1 );
		SFontVal = new FontCombo(Search, Prefs);
		SFontVal->setMaximumSize(190, 30);
		SFontVal->setCurrentText(doc->CurrFont);
		SFontVal->setEnabled(false);
		SearchLayout->addWidget( SFontVal, 2, 1 );
		SSizeVal = new MSpinBox( Search, 1 );
		SSizeVal->setSuffix( tr( " pt" ) );
		SSizeVal->setMinValue( 1 );
		SSizeVal->setMaxValue( 1024 );
		SSizeVal->setValue( doc->CurrFontSize / 10.0 );
		SSizeVal->setEnabled(false);
		SearchLayout->addWidget( SSizeVal, 3, 1 );
		SEffVal = new StyleSelect( Search );
		SEffVal->setStyle(0);
		SEffVal->setEnabled(false);
		SearchLayout->addWidget( SEffVal, 4, 1, Qt::AlignLeft );
		SFillVal = new QComboBox( true, Search, "SFillVal" );
		SFillVal->setEditable(false);
		SFillVal->insertItem( tr("None"));
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			pm.fill(doc->PageColors[it.key()].getRGBColor());
			SFillVal->insertItem(pm, it.key());
		}
		SFillVal->listBox()->setMinimumWidth(SFillVal->listBox()->maxItemWidth()+24);
		SFillVal->setCurrentText(doc->CurrTextFill);
		SFillVal->setEnabled(false);
		SearchLayout->addWidget( SFillVal, 5, 1 );
		SFillSVal = new ShadeButton(Search);
		SFillSVal->setEnabled(false);
		SearchLayout->addWidget( SFillSVal, 6, 1, Qt::AlignLeft );
		SStrokeVal = new QComboBox( true, Search, "SStrokeVal" );
		SStrokeVal->setEditable(false);
		SStrokeVal->insertItem( tr("None"));
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			pm.fill(doc->PageColors[it.key()].getRGBColor());
			SStrokeVal->insertItem(pm, it.key());
		}
		SStrokeVal->listBox()->setMinimumWidth(SStrokeVal->listBox()->maxItemWidth()+24);
		SStrokeVal->setCurrentText(doc->CurrTextStroke);
		SStrokeVal->setEnabled(false);
		SearchLayout->addWidget( SStrokeVal, 7, 1 );
		SStrokeSVal =  new ShadeButton(Search);
		SStrokeSVal->setEnabled(false);
		SearchLayout->addWidget( SStrokeSVal, 8, 1, Qt::AlignLeft );
		SelLayout->addWidget( Search );

		Replace = new QGroupBox( this, "Replace" );
		Replace->setTitle( tr( "Replace with:" ) );
		Replace->setColumnLayout(0, Qt::Vertical );
		Replace->layout()->setSpacing( 2 );
		Replace->layout()->setMargin( 5 );
		ReplaceLayout = new QGridLayout( Replace->layout() );
		ReplaceLayout->setAlignment( Qt::AlignTop );
		RText = new QCheckBox( Replace, "RText" );
		RText->setText( tr( "Text" ) );
		ReplaceLayout->addWidget( RText, 0, 0 );
		RStyle = new QCheckBox( Replace, "RStyle" );
		RStyle->setText( tr( "Paragraph Style" ) );
		ReplaceLayout->addWidget( RStyle, 1, 0 );
		RFont = new QCheckBox( Replace, "RFont" );
		RFont->setText( tr( "Font" ) );
		ReplaceLayout->addWidget( RFont, 2, 0 );
		RSize = new QCheckBox( Replace, "RSize" );
		RSize->setText( tr( "Font Size" ) );
		ReplaceLayout->addWidget( RSize, 3, 0 );
		REffect = new QCheckBox( Replace, "REffect" );
		REffect->setText( tr( "Font Effects" ) );
		ReplaceLayout->addWidget( REffect, 4, 0 );
		RFill = new QCheckBox( Replace, "RFill" );
		RFill->setText( tr( "Fill Color" ) );
		ReplaceLayout->addWidget( RFill, 5, 0 );
		RFillS = new QCheckBox( Replace, "RFillS" );
		RFillS->setText( tr( "Fill Shade" ) );
		ReplaceLayout->addWidget( RFillS, 6, 0 );
		RStroke = new QCheckBox( Replace, "RStroke" );
		RStroke->setText( tr( "Stroke Color" ) );
		ReplaceLayout->addWidget( RStroke, 7, 0 );
		RStrokeS = new QCheckBox( Replace, "RStrokeS" );
		RStrokeS->setText( tr( "Stroke Shade" ) );
		ReplaceLayout->addWidget( RStrokeS, 8, 0 );
		RTextVal = new QLineEdit( Replace, "RTextVal" );
		RTextVal->setEnabled(false);
		ReplaceLayout->addWidget( RTextVal, 0, 1 );
		RStyleVal = new QComboBox( true, Replace, "RStyleVal" );
		RStyleVal->setEditable(false);
		for (uint a = 0; a < ar_sty; ++a)
			RStyleVal->insertItem(tmp_sty[a]);
		if (doc->Vorlagen.count() > 5)
		{
			for (uint x = 5; x < doc->Vorlagen.count(); ++x)
				RStyleVal->insertItem(doc->Vorlagen[x].Vname);
		}
		RStyleVal->listBox()->setMinimumWidth(RStyleVal->listBox()->maxItemWidth()+24);
		RStyleVal->setCurrentItem(doc->CurrentABStil);
		RStyleVal->setEnabled(false);
		ReplaceLayout->addWidget( RStyleVal, 1, 1 );
		RFontVal = new FontCombo(Replace, Prefs);
		RFontVal->setMaximumSize(190, 30);
		RFontVal->setCurrentText(doc->CurrFont);
		RFontVal->setEnabled(false);
		ReplaceLayout->addWidget( RFontVal, 2, 1 );
		RSizeVal = new MSpinBox( Replace, 1 );
		RSizeVal->setSuffix( tr( " pt" ) );
		RSizeVal->setMinValue( 1 );
		RSizeVal->setMaxValue( 1024 );
		RSizeVal->setValue( doc->CurrFontSize / 10.0 );
		RSizeVal->setEnabled(false);
		ReplaceLayout->addWidget( RSizeVal, 3, 1 );
		REffVal = new StyleSelect( Replace );
		REffVal->setStyle(0);
		REffVal->setEnabled(false);
		ReplaceLayout->addWidget( REffVal, 4, 1, Qt::AlignLeft );
		RFillVal = new QComboBox( true, Replace, "RFillVal" );
		RFillVal->setEditable(false);
		RFillVal->insertItem( tr("None"));
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			pm.fill(doc->PageColors[it.key()].getRGBColor());
			RFillVal->insertItem(pm, it.key());
		}
		RFillVal->listBox()->setMinimumWidth(RFillVal->listBox()->maxItemWidth()+24);
		RFillVal->setCurrentText(doc->CurrTextFill);
		RFillVal->setEnabled(false);
		ReplaceLayout->addWidget( RFillVal, 5, 1 );
		RFillSVal = new ShadeButton(Replace);
		RFillSVal->setEnabled(false);
		ReplaceLayout->addWidget( RFillSVal, 6, 1, Qt::AlignLeft );
		RStrokeVal = new QComboBox( true, Replace, "RStrokeVal" );
		RStrokeVal->setEditable(false);
		RStrokeVal->insertItem( tr("None"));
		for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
			pm.fill(doc->PageColors[it.key()].getRGBColor());
			RStrokeVal->insertItem(pm, it.key());
		}
		RStrokeVal->listBox()->setMinimumWidth(RStrokeVal->listBox()->maxItemWidth()+24);
		RStrokeVal->setCurrentText(doc->CurrTextStroke);
		RStrokeVal->setEnabled(false);
		ReplaceLayout->addWidget( RStrokeVal, 7, 1 );
		RStrokeSVal = new ShadeButton(Replace);;
		RStrokeSVal->setEnabled(false);
		ReplaceLayout->addWidget( RStrokeSVal, 8, 1, Qt::AlignLeft );
		SelLayout->addWidget( Replace );
		SearchReplaceLayout->addLayout( SelLayout );
	}
	else
	{
		SearchLayout = new QGridLayout(0, 1, 1, 0, 6 );
		SText1 = new QLabel(this);
		SText1->setText( tr( "Search for:" ) );
		SearchLayout->addWidget( SText1, 0, 0 );
		STextVal = new QLineEdit( this, "STextVal" );
		SearchLayout->addWidget( STextVal, 0, 1 );
		RText1 = new QLabel(this);
		RText1->setText(  tr( "Replace with:" )  );
		SearchLayout->addWidget( RText1, 1, 0 );
		RTextVal = new QLineEdit( this, "RTextVal" );
		SearchLayout->addWidget( RTextVal, 1, 1 );
		SearchReplaceLayout->addLayout( SearchLayout );
	}

	OptsLayout = new QHBoxLayout( 0, 0, 6, "OptsLayout"); 
	Word = new QCheckBox( this, "Word" );
	if (mode)
		Word->setEnabled(false);
	Word->setText( tr( "Whole Word" ) );
	OptsLayout->addWidget( Word );
	CaseIgnore = new QCheckBox( this, "CaseIgnore" );
	CaseIgnore->setText( tr( "Ignore Case" ) );
	if (mode)
		CaseIgnore->setEnabled(false);
	OptsLayout->addWidget( CaseIgnore );
	SearchReplaceLayout->addLayout( OptsLayout );

	ButtonsLayout = new QHBoxLayout( 0, 0, 4, "ButtonsLayout"); 
	DoSearch = new QPushButton( this, "DoSearch" );
	DoSearch->setText( tr( "Search" ) );
	DoSearch->setDefault( true );
	ButtonsLayout->addWidget( DoSearch );
	DoReplace = new QPushButton( this, "DoReplace" );
	DoReplace->setText( tr( "Replace" ) );
	DoReplace->setEnabled(false);
	ButtonsLayout->addWidget( DoReplace );
	AllReplace = new QPushButton( this, "DoReplace" );
	AllReplace->setText( tr( "Replace All" ) );
	AllReplace->setEnabled(false);
	ButtonsLayout->addWidget( AllReplace );
	Leave = new QPushButton( this, "Leave" );
	Leave->setText( tr( "Leave" ) );
	ButtonsLayout->addWidget( Leave );
	SearchReplaceLayout->addLayout( ButtonsLayout );

	resize(minimumSizeHint());

 // signals and slots connections
	connect( Leave, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( DoSearch, SIGNAL( clicked() ), this, SLOT( slotSearch() ) );
	connect( DoReplace, SIGNAL( clicked() ), this, SLOT( slotReplace() ) );
	connect( AllReplace, SIGNAL( clicked() ), this, SLOT( slotReplaceAll() ) );
	if (mode)
	{
		connect( SText, SIGNAL( clicked() ), this, SLOT( enableTxSearch() ) );
		connect( SStyle, SIGNAL( clicked() ), this, SLOT( enableStyleSearch() ) );
		connect( SFont, SIGNAL( clicked() ), this, SLOT( enableFontSearch() ) );
		connect( SSize, SIGNAL( clicked() ), this, SLOT( enableSizeSearch() ) );
		connect( SEffect, SIGNAL( clicked() ), this, SLOT( enableEffSearch() ) );
		connect( SFill, SIGNAL( clicked() ), this, SLOT( enableFillSearch() ) );
		connect( SFillS, SIGNAL( clicked() ), this, SLOT( enableFillSSearch() ) );
		connect( SStrokeS, SIGNAL( clicked() ), this, SLOT( enableStrokeSSearch() ) );
		connect( SStroke, SIGNAL( clicked() ), this, SLOT( enableStrokeSearch() ) );
		connect( RText, SIGNAL( clicked() ), this, SLOT( enableTxReplace() ) );
		connect( RStyle, SIGNAL( clicked() ), this, SLOT( enableStyleReplace() ) );
		connect( RFont, SIGNAL( clicked() ), this, SLOT( enableFontReplace() ) );
		connect( RSize, SIGNAL( clicked() ), this, SLOT( enableSizeReplace() ) );
		connect( REffect, SIGNAL( clicked() ), this, SLOT( enableEffReplace() ) );
		connect( RFill, SIGNAL( clicked() ), this, SLOT( enableFillReplace() ) );
		connect( RStroke, SIGNAL( clicked() ), this, SLOT( enableStrokeReplace() ) );
		connect( RFillS, SIGNAL( clicked() ), this, SLOT( enableFillSReplace() ) );
		connect( RStrokeS, SIGNAL( clicked() ), this, SLOT( enableStrokeSReplace() ) );
	}

 // tab order
	if (mode)
	{
		setTabOrder( SText, SStyle );
		setTabOrder( SStyle, SFont );
		setTabOrder( SFont, SSize );
		setTabOrder( SSize, SEffect );
		setTabOrder( SEffect, SFill );
		setTabOrder( SFill, SStroke );
		setTabOrder( SStroke, STextVal );
		setTabOrder( STextVal, SStyleVal );
		setTabOrder( SStyleVal, SFontVal );
		setTabOrder( SFontVal, SSizeVal );
		setTabOrder( SSizeVal, SEffVal );
		setTabOrder( SEffVal, SFillVal );
		setTabOrder( SFillVal, SStrokeVal );
		setTabOrder( SStrokeVal, RText );
		setTabOrder( RText, RStyle );
		setTabOrder( RStyle, RFont );
		setTabOrder( RFont, RSize );
		setTabOrder( RSize, REffect );
		setTabOrder( REffect, RFill );
		setTabOrder( RFill, RStroke );
		setTabOrder( RStroke, RTextVal );
		setTabOrder( RTextVal, RStyleVal );
		setTabOrder( RStyleVal, RFontVal );
		setTabOrder( RFontVal, RSizeVal );
		setTabOrder( RSizeVal, REffVal );
		setTabOrder( REffVal, RFillVal );
		setTabOrder( RFillVal, RStrokeVal );
		setTabOrder( RStrokeVal, Word );
		setTabOrder( Word, CaseIgnore );
		setTabOrder( CaseIgnore, DoSearch );
		setTabOrder( DoSearch, DoReplace );
		setTabOrder( DoReplace, AllReplace );
		setTabOrder( AllReplace, Leave );
	}
}

void SearchReplace::slotSearch()
{
	if (SMode)
	{
		Doc->ActPage->slotDoCurs(false);
		slotDoSearch();
		Doc->ActPage->slotDoCurs(true);
		Doc->ActPage->RefreshItem(Item, true);
	}
}

void SearchReplace::slotDoSearch()
{
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	for (uint a = 0; a < Item->Ptext.count(); ++a)
		Item->Ptext.at(a)->cselect = false;
	Item->HasSel = false;
	QString fCol = "";
	QString sCol = "";
	QString sFont = "";
	QString sText = "";
	NotFound = true;
	int sStyle = 0;
	int sSize = 0;
	int sEff = 0;
	int sFillSh = 100;
	int sStrokeSh = 100;
	bool rep = false;
	bool found = true;
	if ((RFill->isChecked()) || (RStroke->isChecked()) || (RStyle->isChecked()) || (RFont->isChecked())
		|| (RStrokeS->isChecked()) || (RFillS->isChecked()) || (RSize->isChecked()) || (RText->isChecked())
		|| (REffect->isChecked()))
		rep = true;
	if (SText->isChecked())
		sText = STextVal->text();
	if (CaseIgnore->isChecked())
		sText = sText.lower();
	if (SEffect->isChecked())
		sEff = SEffVal->getStyle();
	if (SFill->isChecked())
		fCol = SFillVal->currentText();
	if (SFillS->isChecked())
		sFillSh = SFillSVal->getValue();
	if (SStroke->isChecked())
		sCol = SStrokeVal->currentText();
	if (SStrokeS->isChecked())
		sStrokeSh = SStrokeSVal->getValue();
	if (SFont->isChecked())
		sFont = SFontVal->currentText();
	if (SStyle->isChecked())
		sStyle = SStyleVal->currentItem();
	if (SSize->isChecked())
		sSize = qRound(SSizeVal->value() * 10);
	if (sText.length() > 0)
		found = false;
	uint inde = 0;
	uint as = Item->CPos;
	ReplStart = as;
	uint a;
	for (a = as; a < Item->Ptext.count(); ++a)
	{
		if (SText->isChecked())
		{
			QString chx = Item->Ptext.at(a)->ch;
			if (CaseIgnore->isChecked())
				chx = chx.lower();
			found = chx == sText.mid(inde, 1) ? true : false;
			if ((Word->isChecked()) && (inde == 0) && (chx[0].isSpace()))
				found = true;
		}
		else
			found = true;
		if (SSize->isChecked())
		{
			if (Item->Ptext.at(a)->csize != sSize)
				found = false;
		}
		if (SFont->isChecked())
		{
			if (Item->Ptext.at(a)->cfont != sFont)
				found = false;
		}
		if (SStyle->isChecked())
		{
			if (Item->Ptext.at(a)->cab != sStyle)
				found = false;
		}
		if (SStroke->isChecked())
		{
			if (Item->Ptext.at(a)->cstroke != sCol)
				found = false;
		}
		if (SStrokeS->isChecked())
		{
			if (Item->Ptext.at(a)->cshade2 != sStrokeSh)
				found = false;
		}
		if (SFillS->isChecked())
		{
			if (Item->Ptext.at(a)->cshade != sFillSh)
				found = false;
		}
		if (SEffect->isChecked())
			{
			if ((Item->Ptext.at(a)->cstyle & 127) != sEff)
				found = false;
			}
		if (SFill->isChecked())
		{			
			if (Item->Ptext.at(a)->ccolor != fCol)
				found = false;
		}
		if (found)
		{
			Item->Ptext.at(a)->cselect = true;
			Item->HasSel = true;
			if (rep)
			{
				DoReplace->setEnabled(true);
				AllReplace->setEnabled(true);
			}
			Item->CPos = a+1;
			if (SText->isChecked())
			{
				if (inde == 0)
					ReplStart = a;
				inde++;
				if ((Word->isChecked()) && (inde == 1) && (Item->Ptext.at(a)->ch[0].isSpace()))
				{
					inde--;
					Item->Ptext.at(a)->cselect = false;
				}
				if ((Word->isChecked()) && (inde == sText.length()) &&
					 (!Item->Ptext.at(QMIN(Item->MaxChars-1,a+1))->ch[0].isSpace()))
				{
					for (uint xx = ReplStart; xx < a+1; ++xx)
						Item->Ptext.at(QMIN(xx,Item->MaxChars-1))->cselect = false;
					Item->HasSel = false;
					inde = 0;
					found = false;
				}
				else
				{
					if (inde == sText.length())
						break;
				}
			}
			else
				break;
		}
		else
		{
			if (SText->isChecked())
			{
				for (uint xx = ReplStart; xx < a+1; ++xx)
					Item->Ptext.at(QMIN(xx,Item->MaxChars-1))->cselect = false;
				Item->HasSel = false;
			}
			inde = 0;
		}
	}
	if ((!found) || (a == Item->Ptext.count()))
	{
		Doc->DoDrawing = true;
		Doc->ActPage->RefreshItem(Item, true);
		DoReplace->setEnabled(false);
		AllReplace->setEnabled(false);
		QMessageBox::information(this, tr("Search/Replace"), tr("Search finished"), tr("OK"));
		Item->CPos = 0;
		NotFound = false;
	}
}

void SearchReplace::slotReplace()
{
	if (SMode)
	{
		Doc->ActPage->slotDoCurs(false);
		slotDoReplace();
		Doc->ActPage->slotDoCurs(true);
		Doc->ActPage->RefreshItem(Item, true);
	}
}

void SearchReplace::slotDoReplace()
{
	QString repl, sear;
	uint cs, cx;
	struct Pti *hg;
	if (RText->isChecked())
	{
		repl = RTextVal->text();
		sear = STextVal->text();
		if (sear.length() == repl.length())
		{
			for (cs = 0; cs < sear.length(); ++cs)
				Item->Ptext.at(ReplStart+cs)->ch = repl[cs];
		}
		else
		{
			if (sear.length() < repl.length())
			{
				for (cs = 0; cs < sear.length(); ++cs)
					Item->Ptext.at(ReplStart+cs)->ch = repl[cs];
				for (cx = cs; cx < repl.length(); ++cx)
				{
					hg = new Pti;
					hg->ch = repl[cx];
					if (RSize->isChecked())
						hg->csize = qRound(RSizeVal->value() * 10.0);
					else
 						hg->csize = Doc->CurrFontSize;
					if (RFill->isChecked())
						hg->ccolor = RFillVal->currentText();
					else
						hg->ccolor = Doc->CurrTextFill;
					hg->cshade = Doc->CurrTextFillSh;
					if (RStroke->isChecked())
						hg->cstroke = RStrokeVal->currentText();
					else
						hg->cstroke = Doc->CurrTextStroke;
					hg->cshade2 = Doc->CurrTextStrokeSh;
					hg->cscale = Doc->CurrTextScale;
 					hg->cselect = true;
 					hg->cstyle = Doc->CurrentStyle;
					if (RStyle->isChecked())
						hg->cab = RStyleVal->currentItem();
					else
 						hg->cab = Doc->CurrentABStil;
					if (Doc->Vorlagen[hg->cab].Font != "")
					{
						hg->cfont = Doc->Vorlagen[hg->cab].Font;
						hg->csize = Doc->Vorlagen[hg->cab].FontSize;
						hg->cstyle = Doc->Vorlagen[hg->cab].FontEffect;
					}
					if (RFont->isChecked())
						hg->cfont = RFontVal->currentText();
					else
 						hg->cfont = Doc->CurrFont;
 					hg->cextra = 0;
 					hg->xp = 0;
 					hg->yp = 0;
					hg->PRot = 0;
					hg->PtransX = 0;
					hg->PtransY = 0;
 					Item->Ptext.insert(ReplStart+cx, hg);     
				}
				Item->CPos = ReplStart+cx;
			}
			else
			{
				for (cs = 0; cs < repl.length(); ++cs)
					Item->Ptext.at(ReplStart+cs)->ch = repl[cs];
				for (uint cxx = cs; cxx < sear.length(); ++cxx)
 					Item->Ptext.remove(ReplStart+cs);
				Item->CPos = ReplStart+cs;
			}
		}
	}
	if (RStyle->isChecked())
		emit NewAbs(RStyleVal->currentItem());
	if (RFill->isChecked())
		Doc->ActPage->ItemTextBrush(RFillVal->currentText());
	if (RFillS->isChecked())
		Doc->ActPage->ItemTextBrushS(RFillSVal->getValue());
	if (RStroke->isChecked())
		Doc->ActPage->ItemTextPen(RStrokeVal->currentText());
	if (RStrokeS->isChecked())
		Doc->ActPage->ItemTextPenS(RStrokeSVal->getValue());
	if (RFont->isChecked())
		emit NewFont(RFontVal->currentText());
	if (RSize->isChecked())
		Doc->ActPage->chFSize(qRound(RSizeVal->value() * 10.0));
	if (REffect->isChecked())
		{
		int s = REffVal->getStyle();
		Doc->CurrentStyle = s;
		if (Item->Ptext.count() != 0)
			{
			for (uint a = 0; a < Item->Ptext.count(); ++a)
				{
				if (Item->Ptext.at(a)->cselect)
					{
					Item->Ptext.at(a)->cstyle &= ~127;
					Item->Ptext.at(a)->cstyle |= s;
					}
				}
			}
		}
	DoReplace->setEnabled(false);
	AllReplace->setEnabled(false);
	for (uint a = 0; a < Item->Ptext.count(); ++a)
		Item->Ptext.at(a)->cselect = false;
}

void SearchReplace::slotReplaceAll()
{
	if (SMode)
	{
		Doc->ActPage->slotDoCurs(false);
		Doc->DoDrawing = false;
		do
		{
			slotDoReplace();
			slotDoSearch();
		}
		while (NotFound);
		Doc->DoDrawing = true;
		Doc->ActPage->slotDoCurs(true);
		Doc->ActPage->RefreshItem(Item, true);
	}
}

void SearchReplace::enableTxSearch()
{
	bool setter = SText->isChecked();
	STextVal->setEnabled(setter);
	Word->setEnabled(setter);
	CaseIgnore->setEnabled(setter);
}

void SearchReplace::enableStyleSearch()
{
	SStyleVal->setEnabled(SStyle->isChecked());
}

void SearchReplace::enableFontSearch()
{
	SFontVal->setEnabled(SFont->isChecked());
}

void SearchReplace::enableSizeSearch()
{
	SSizeVal->setEnabled(SSize->isChecked());
}

void SearchReplace::enableEffSearch()
{
	SEffVal->setEnabled(SEffect->isChecked());
}

void SearchReplace::enableFillSearch()
{
	SFillVal->setEnabled(SFill->isChecked());
}

void SearchReplace::enableFillSSearch()
{
	SFillSVal->setEnabled(SFillS->isChecked());
}

void SearchReplace::enableStrokeSearch()
{
	SStrokeVal->setEnabled(SStroke->isChecked());
}

void SearchReplace::enableStrokeSSearch()
{
	SStrokeSVal->setEnabled(SStrokeS->isChecked());
}

void SearchReplace::enableTxReplace()
{
	RTextVal->setEnabled(RText->isChecked());
}

void SearchReplace::enableStyleReplace()
{
	RStyleVal->setEnabled(RStyle->isChecked());
}

void SearchReplace::enableFontReplace()
{
	RFontVal->setEnabled(RFont->isChecked());
}

void SearchReplace::enableSizeReplace()
{
	RSizeVal->setEnabled(RSize->isChecked());
}

void SearchReplace::enableEffReplace()
{
	REffVal->setEnabled(REffect->isChecked());
}

void SearchReplace::enableFillReplace()
{
	RFillVal->setEnabled(RFill->isChecked());
}

void SearchReplace::enableFillSReplace()
{
	RFillSVal->setEnabled(RFillS->isChecked());
}

void SearchReplace::enableStrokeReplace()
{
	RStrokeVal->setEnabled(RStroke->isChecked());
}

void SearchReplace::enableStrokeSReplace()
{
	RStrokeSVal->setEnabled(RStrokeS->isChecked());
}

