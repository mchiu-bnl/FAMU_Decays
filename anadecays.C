//
//
//

TH1 *hrawspect{nullptr};
TGraphErrors *g_cts;
TGraphErrors *g_dcts;
TGraphErrors *g_lcts;
vector<Double_t> cts;
vector<Double_t> cts_err;
vector<Double_t> timebin;
vector<Double_t> dcts;        // dark counts
vector<Double_t> dcts_err;
vector<Double_t> dtimebin;
vector<Double_t> lcts;        // light counts
vector<Double_t> lcts_err;
vector<Double_t> ltimebin;

int countwords(const std::string &s)
{
  std::istringstream iss( s );
  return std::distance(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{});
}

//void makehist(const std::string& fname = "PHHvT_121824_sP_BH-PolV-D1779-B1765-Cs-FSU-1d.txt")
//void makehist(const std::string& fname = "PHHvT_123024_sP-sD-15min-Part4b-2BH7Lv850-P1HP2V_-D4B1740.txt")
void makehist(const std::string& fname = "PHHvT_Tally_laser1_020725_sP-sD_B45-PolPar_D1781_B1761_stepmotor_0001.txt")
{
  ifstream infile(fname);

  g_cts = new TGraphErrors();

  int nbins{0};
  int nline{0};
  double ctsintimebin{ 0. };
  std::string line;

  while ( std::getline( infile, line) )
  {
    if ( nline==4800 )
    {
      break;
    }

    // get the number of words in string
    // this is the number of bins in the spectrum
    if ( nbins==0 )
    {
      nbins = countwords( line );
      cout << nbins << endl;

      hrawspect = new TH1F("hrawspect","raw spectrum",nbins,0.,nbins);
      hrawspect->SetXTitle("pulse height (a.u.)");
      hrawspect->SetYTitle("counts");
      hrawspect->SetLineColor(4);
    }

    int ibin{ 0 };
    int tempval{ 0 };

    if ( nline%50 == 0 )
    {
      ctsintimebin = 0.;
    }

    std::istringstream ss( line );

    while ( ss >> tempval )
    { 
      hrawspect->Fill( ibin+0.5, tempval );
      ctsintimebin += tempval;
      ibin++;
      //cout << ibin << "\t" << tempval << endl;
    }

    nline++;

    if ( nline%50 == 0 )
    {
      timebin.push_back( cts.size() );
      cout << cts.size() << endl;
      double e_cts = sqrt( ctsintimebin );
      cts.push_back( ctsintimebin );
      cts_err.push_back( e_cts );

      if ( ((nline-1)/50)%2 == 0 )
      {
        lcts.push_back( ctsintimebin );
        lcts_err.push_back( e_cts );
        ltimebin.push_back( cts.size()-1 );
      }
      else
      {
        dcts.push_back( ctsintimebin );
        dcts_err.push_back( e_cts );
        dtimebin.push_back( cts.size()-1 );
      }
    }
  }

  TCanvas *ac[20];
  int icv = 0;
  ac[icv] = new TCanvas("ac0","spectrum",800,600);
  hrawspect->Draw();
  gPad->SetLogy(1);
  icv++;


  ac[icv] = new TCanvas("ac1","spectrum",1200,600);
  g_cts = new TGraphErrors( cts.size(), timebin.data(), cts.data(), 0, cts_err.data() );
  g_cts->SetMarkerStyle(20);
  g_cts->Draw("ap");

  g_lcts = new TGraphErrors( lcts.size(), ltimebin.data(), lcts.data(), 0, lcts_err.data() );
  g_lcts->SetMarkerStyle(20);
  g_lcts->SetMarkerColor(2);
  g_lcts->SetLineColor(2);
  g_lcts->Draw("p");

  g_dcts = new TGraphErrors( dcts.size(), dtimebin.data(), dcts.data(), 0, dcts_err.data() );
  g_dcts->SetMarkerStyle(20);
  g_dcts->SetMarkerColor(4);
  g_dcts->SetLineColor(4);
  g_dcts->Draw("p");

  icv++;


  for (int ibin=1; ibin<20; ibin++)
  {
    cout << ibin << "\t" << hrawspect->GetBinContent(ibin) << endl;
  }
}
