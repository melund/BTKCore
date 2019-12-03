#ifndef C3DFileWriterTest_h
#define C3DFileWriterTest_h

#include <btkAcquisitionFileReader.h>
#include <btkAcquisitionFileWriter.h>
#include <btkC3DFileIO.h>
#include <btkConvert.h>

CXXTEST_SUITE(C3DFileWriterTest)
{
  CXXTEST_TEST(NoFileNoInput)
  {
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(NoFileWithIO)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    btk::C3DFileIO::Pointer io = btk::C3DFileIO::New();
    writer->SetAcquisitionIO(io);
    TS_ASSERT(io->HasInternalsUpdateOption(btk::C3DFileIO::DataBasedUpdate));
    TS_ASSERT(io->HasInternalsUpdateOption(btk::C3DFileIO::CompatibleVicon));
    TS_ASSERT_THROWS_EQUALS(writer->Update(), const btk::AcquisitionFileWriterException &e, e.what(), std::string("Filename must be specified."));
  };

  CXXTEST_TEST(sample01_Eb015pi_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pi.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015pi.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);

    
    btk::Acquisition::Pointer acqRef = reader->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acqRef->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acqRef->GetAnalog(1)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acqRef->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acqRef->GetAnalog(1)->GetValues()(i), 0.00001);
    }

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);

    TS_ASSERT_EQUALS(acq->GetEventNumber(), 3);
    TS_ASSERT_EQUALS(acq->GetEvent(0)->GetLabel(), "RIC");
    TS_ASSERT_DELTA(acq->GetEvent(0)->GetTime(), 2.72, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(1)->GetLabel(), "RHS");
    TS_ASSERT_DELTA(acq->GetEvent(1)->GetTime(), 5.40, 0.0001);
    TS_ASSERT_EQUALS(acq->GetEvent(2)->GetLabel(), "RTO");
    TS_ASSERT_DELTA(acq->GetEvent(2)->GetTime(), 7.32, 0.0001);
    
    TS_ASSERT_EQUALS((int)acq->GetMetaData()->GetChild("EVENT")->GetChild("TIMES")->GetInfo()->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("EVENT")->GetChild("TIMES")->GetInfo()->GetDimensions()[0], 2);
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("EVENT")->GetChild("TIMES")->GetInfo()->GetDimensions()[1], 3);

    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("ANALOG")->GetChild("OFFSET")->GetInfo()->GetFormat(), btk::MetaDataInfo::Integer);
  };

  CXXTEST_TEST(sample01_Eb015pr_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetInternalsUpdateOptions(btk::C3DFileIO::NoUpdate);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pr.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015pi.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);

    btk::Acquisition::Pointer acqRef = reader->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(i), acqRef->GetPoint(0)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(i), acqRef->GetPoint(14)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acqRef->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acqRef->GetAnalog(1)->GetValues()(i), 0.00001);
    }

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015si.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetInternalsUpdateOptions(btk::C3DFileIO::NoUpdate);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015si.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);
    
    btk::Acquisition::Pointer acqRef = reader->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(i), acqRef->GetPoint(0)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(i), acqRef->GetPoint(14)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acqRef->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acqRef->GetAnalog(1)->GetValues()(i), 0.00001);
    }
    
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015sr_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015sr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetInternalsUpdateOptions(btk::C3DFileIO::NoUpdate);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015sr.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015sr.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);
    
    btk::Acquisition::Pointer acqRef = reader->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(i), acqRef->GetPoint(0)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(i), acqRef->GetPoint(14)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acqRef->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acqRef->GetAnalog(1)->GetValues()(i), 0.00001);
    }

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vi_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetInternalsUpdateOptions(btk::C3DFileIO::NoUpdate);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vi.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015vi.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);
    
    btk::Acquisition::Pointer acqRef = reader->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(i), acqRef->GetPoint(0)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(i), acqRef->GetPoint(14)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acqRef->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acqRef->GetAnalog(1)->GetValues()(i), 0.00001);
    }

    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetInternalsUpdateOptions(btk::C3DFileIO::NoUpdate);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015vr.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 26);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetAnalog(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);

    btk::Acquisition::Pointer acqRef = reader->GetOutput();
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(i), acqRef->GetPoint(0)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(i), acqRef->GetPoint(14)->GetResiduals()(i), 0.0001);
      TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(i), acqRef->GetAnalog(0)->GetValues()(i), 0.00001);
      TS_ASSERT_DELTA(acq->GetAnalog(1)->GetValues()(i), acqRef->GetAnalog(1)->GetValues()(i), 0.00001);
    }
    
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_clearAnalog)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAnalog.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAnalog.c3d");
    reader->Update();
    acq = reader->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPoint(14)->HasParent(), true);
    TS_ASSERT_EQUALS(acq->GetMetaData()->HasParent(), true);

    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(writer->GetAcquisitionIO());
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_clearPoint)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearPoints();
    acq->ClearEvents();
    acq->SetMaxInterpolationGap(50);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearPoint.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearPoint.c3d");
    reader->Update();
    acq = reader->GetOutput();
    TS_ASSERT_EQUALS(acq->GetMaxInterpolationGap(), 50);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 16);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalog(15)->GetLabel(), "CH16");
    TS_ASSERT_EQUALS(acq->GetAnalog(2)->GetDescription(), "FORCE Z-COMP 1");
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0,0), -26.6599, 0.001);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1,0), -25.8, 0.0001);

    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(writer->GetAcquisitionIO());
    TS_ASSERT_DELTA(io->GetPointScale(), 0.1, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_clearAll)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearPoints();
    acq->ClearAnalogs();
    acq->ClearEvents();
    writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAll.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_clearAll.c3d");
    reader->Update();
    acq = reader->GetOutput();
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);

    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015pi_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vi_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015pi_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vi_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Integer);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vi_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015si_from_Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pi.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015si_from_Eb015pi.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_from_Eb015pi)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Integer);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015pi.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015pi.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015vr_from_Eb015sr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015sr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::VAX_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015sr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015vr_from_Eb015sr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015sr_from_Eb015pr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_BigEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015sr_from_Eb015pr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015sr_from_Eb015pr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_BigEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample01_Eb015pr_from_Eb015vr)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015vr.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->ClearAnalogs();
    acq->ClearEvents();
    acq->RemovePoint(25);
    acq->RemovePoint(24);
    acq->RemovePoint(23);
    acq->RemovePoint(22);
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::VAX_LittleEndian);
    io->SetStorageFormat(btk::C3DFileIO::Float);
    io->SetByteOrder(btk::AcquisitionFileIO::IEEE_LittleEndian);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pr_from_Eb015vr.c3d");
    writer->Update();

    reader->SetFilename(C3DFilePathOUT + "sample01_Eb015pr_from_Eb015vr.c3d");
    reader->Update();
    acq = reader->GetOutput();
    io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    TS_ASSERT_EQUALS(io->GetStorageFormat(), btk::C3DFileIO::Float);
    TS_ASSERT_EQUALS(io->GetByteOrder(), btk::AcquisitionFileIO::IEEE_LittleEndian);
    TS_ASSERT_DELTA(io->GetPointScale(), 0.08333, 0.0001);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 50);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 22);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetEventNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel(), "RFT1");
    TS_ASSERT_EQUALS(acq->GetPoint(9)->GetDescription(), "DISTAL FOOT");
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetValues()(8,0), 250.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,0), 285, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,1), 222.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(17)->GetValues()(16,2), 564.4166, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(0)->GetResiduals()(0,0), 1.3333, 0.0001);
    TS_ASSERT_DELTA(acq->GetPoint(14)->GetResiduals()(20), 1.91667, 0.0001);
  };

  CXXTEST_TEST(sample09_PlugInC3D_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    reader2->Update();

    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    TS_ASSERT_DELTA(acq2->GetMetaData()->GetChild("ANALOG")->GetChild("RATE")->GetInfo()->ToFloat(0), acq->GetAnalogFrequency(), 1e-5);
    
    for (int i = 1 ; i < 50 ; ++i)
    {
      for (int j = 0 ; j < acq->GetPointNumber() ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,0), acq2->GetPoint(j)->GetValues()(i,0), 0.00001);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,1), acq2->GetPoint(j)->GetValues()(i,1), 0.00001);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetValues()(i,2), acq2->GetPoint(j)->GetValues()(i,2), 0.00001);
        TS_ASSERT_DELTA(acq->GetPoint(j)->GetResiduals()(i), acq2->GetPoint(j)->GetResiduals()(i), 0.00001);
      }
      for (int j = 0 ; j < acq->GetAnalogNumber() ; ++j)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(j)->GetValues()(i), acq2->GetAnalog(j)->GetValues()(i), 0.00005); // 5e-5 instead of 1e-5 due to the use of the general scale factor
      }
    }
    
    TS_ASSERT_EQUALS(acq2->GetAnalog(0)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(1)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(2)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(3)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(4)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(5)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(6)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(7)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(8)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(9)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(10)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(11)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(12)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(13)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(14)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(15)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(16)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(17)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(18)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(19)->GetGain(), btk::Analog::PlusMinus10);
    TS_ASSERT_EQUALS(acq2->GetAnalog(20)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(21)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(22)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(23)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(24)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(25)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(26)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(27)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(28)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(29)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(30)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(31)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(32)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(33)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(34)->GetGain(), btk::Analog::PlusMinus5);
    TS_ASSERT_EQUALS(acq2->GetAnalog(35)->GetGain(), btk::Analog::PlusMinus5);
  }

  CXXTEST_TEST(sample19_sample19_rewrited)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample19/sample19.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample19_sample19.c3d");
    //writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample19_sample19.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    for (int i = 0 ; i < acq->GetAnalogNumber() ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), acq2->GetAnalog(i)->GetLabel());
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetOffset(), acq2->GetAnalog(i)->GetOffset());
      TS_ASSERT_DELTA(acq->GetAnalog(i)->GetScale(), acq2->GetAnalog(i)->GetScale(), 1e-5);
      for (int j = 0 ; j < 1000 ; j+=50)
      {
        TS_ASSERT_DELTA(acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 1e-5);
      }
    }
    
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq->GetMetaData()->GetChild("POINT")->GetChild("FRAMES")->GetInfo()->ToUInt16(0));
    btk::MetaDataInfo::Pointer actualFieldVal = acq->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_START_FIELD")->GetInfo();
    unsigned frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
    TS_ASSERT_EQUALS((unsigned)acq->GetFirstFrame(), frameIndex);
    actualFieldVal = acq->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_END_FIELD")->GetInfo();
    frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
    TS_ASSERT_EQUALS((unsigned)acq->GetLastFrame(), frameIndex);
  };
    
  CXXTEST_TEST(emptyAcquisition_Template)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(C3DFilePathOUT + "EmptyAcquisition.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathOUT + "EmptyAcquisition.c3d");
    reader->Update();
    acq = reader->GetOutput();
    btk::MetaData::Pointer metadata = acq->GetMetaData();
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 0.0);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 0.0);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");
    btk::MetaData::ConstIterator itPoint = metadata->FindChild("POINT");
    btk::MetaData::ConstIterator itAnalog = metadata->FindChild("ANALOG");
    btk::MetaData::ConstIterator itFP = metadata->FindChild("FORCE_PLATFORM");
    TS_ASSERT(itPoint != metadata->End());
    TS_ASSERT(itAnalog != metadata->End());
    TS_ASSERT(itFP != metadata->End());
    // POINT and ANALOG are supposed to be OK
    btk::MetaData::ConstIterator itUsed = (*itFP)->FindChild("USED");
    btk::MetaData::ConstIterator itType = (*itFP)->FindChild("TYPE");
    btk::MetaData::ConstIterator itZero = (*itFP)->FindChild("ZERO");
    btk::MetaData::ConstIterator itCorners = (*itFP)->FindChild("CORNERS");
    btk::MetaData::ConstIterator itOrigin = (*itFP)->FindChild("ORIGIN");
    btk::MetaData::ConstIterator itChannel = (*itFP)->FindChild("CHANNEL");
    TS_ASSERT(itUsed != (*itFP)->End());
    TS_ASSERT(itType != (*itFP)->End());
    TS_ASSERT(itZero != (*itFP)->End());
    TS_ASSERT(itCorners != (*itFP)->End());
    TS_ASSERT(itOrigin != (*itFP)->End());
    TS_ASSERT(itChannel != (*itFP)->End());
    TS_ASSERT((*itUsed)->HasInfo());
    TS_ASSERT((*itType)->HasInfo());
    TS_ASSERT((*itZero)->HasInfo());
    TS_ASSERT((*itCorners)->HasInfo());
    TS_ASSERT((*itOrigin)->HasInfo());
    TS_ASSERT((*itChannel)->HasInfo());
    TS_ASSERT_EQUALS((*itUsed)->GetInfo()->GetDimensionsProduct(), 1);
    TS_ASSERT_EQUALS((*itType)->GetInfo()->GetDimensionsProduct(), 0);
    TS_ASSERT_EQUALS((*itZero)->GetInfo()->GetDimensionsProduct(), 2);
    TS_ASSERT_EQUALS((*itCorners)->GetInfo()->GetDimensionsProduct(), 0);
    TS_ASSERT_EQUALS((*itOrigin)->GetInfo()->GetDimensionsProduct(), 0);
    TS_ASSERT_EQUALS((*itChannel)->GetInfo()->GetDimensionsProduct(), 0);
    TS_ASSERT_EQUALS((int)(*itUsed)->GetInfo()->GetDimensions().size(), 0);
    TS_ASSERT_EQUALS((int)(*itType)->GetInfo()->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS((int)(*itZero)->GetInfo()->GetDimensions().size(), 1);
    TS_ASSERT_EQUALS((int)(*itCorners)->GetInfo()->GetDimensions().size(), 3);
    TS_ASSERT_EQUALS((int)(*itOrigin)->GetInfo()->GetDimensions().size(), 2);
    TS_ASSERT_EQUALS((int)(*itChannel)->GetInfo()->GetDimensions().size(), 2);
  };

  CXXTEST_TEST(convertTRC2C3D)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(TRCFilePathIN + "Knee.trc");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "Knee_rewritedInC3D.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "Knee_rewritedInC3D.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_DELTA(acq->GetPoint(70)->GetResiduals()(82), -1.0, 0.000001);

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    for (int i = 1 ; i < 50 ; ++i)
    {
      TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(i,0), acq2->GetPoint(1)->GetValues()(i,0), 0.00005);
      TS_ASSERT_DELTA(acq->GetPoint(2)->GetValues()(i,1), acq2->GetPoint(2)->GetValues()(i,1), 0.00001);
      TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(i,2), acq2->GetPoint(3)->GetValues()(i,2), 0.00001);
      TS_ASSERT_DELTA(acq->GetPoint(4)->GetResiduals()(i), acq2->GetPoint(4)->GetResiduals()(i), 0.00001);
    }

    TS_ASSERT_DELTA(acq->GetPoint(70)->GetResiduals()(82), acq2->GetPoint(70)->GetResiduals()(82), 0.000001);
  };
  
  
  CXXTEST_TEST(noInputModification)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pr.c3d");
    reader->Update();
    btk::Acquisition::Pointer output = reader->GetOutput();
    unsigned long timestamp = output->GetTimestamp();
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pr.c3d");
    writer->Update();
    TS_ASSERT_EQUALS(output->GetTimestamp(),timestamp);
  }
  
  CXXTEST_TEST(acq100000)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(3,100000,1,2);
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(C3DFilePathOUT + "acq100000.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathOUT + "acq100000.c3d");
    reader->Update();
    btk::Acquisition::Pointer output = reader->GetOutput();
    
    TS_ASSERT_EQUALS(output->GetPointFrameNumber(),100000);
    TS_ASSERT_EQUALS(output->GetAnalogFrameNumber(),200000);
    TS_ASSERT_EQUALS(output->GetPointNumber(),3);
    TS_ASSERT_EQUALS(output->GetAnalogNumber(),1);
    
    TS_ASSERT_EQUALS(output->GetMetaData()->GetChild("POINT")->GetChild("FRAMES")->GetInfo()->ToUInt16(0), 65535);
    btk::MetaDataInfo::Pointer actualFieldVal = output->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_START_FIELD")->GetInfo();
    int frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
    TS_ASSERT_EQUALS(output->GetFirstFrame(), frameIndex);
    actualFieldVal = output->GetMetaData()->GetChild("TRIAL")->GetChild("ACTUAL_END_FIELD")->GetInfo();
    frameIndex = (actualFieldVal->ToUInt16(1) << 16) | actualFieldVal->ToUInt16(0);
    TS_ASSERT_EQUALS(output->GetLastFrame(), frameIndex);
    TS_ASSERT_EQUALS(output->GetLastFrame(), 100000);
  }
  
  CXXTEST_TEST(convertDelsysEMG2C3D)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(DelsysEMGFilePathIN + "Set1[Rep2]_v3.emg");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "Set1[Rep2]_v3_rewritedInC3D.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "Set1[Rep2]_v3_rewritedInC3D.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());

    for(int i = 0 ; i < 8 ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), acq2->GetAnalog(i)->GetLabel());
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetUnit(), acq2->GetAnalog(i)->GetUnit());
      TS_ASSERT_DELTA(acq->GetAnalog(i)->GetScale(), acq2->GetAnalog(i)->GetScale(), 5e-15);
      for(int j = 0 ; j < 60000 ; j+=100)
      {
        TSM_ASSERT_DELTA("Channel #" + btk::ToString(i) + " - Sample " + btk::ToString(j), acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 5e-11);
      }
    }
  };
  
  CXXTEST_TEST(convertCLB2C3D)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(CLBFilePathIN + "NoScale.clb");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "NoScale_rewritedInC3D.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "NoScale_rewritedInC3D.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());

    for(int i = 0 ; i < 10 ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel(), acq2->GetAnalog(i)->GetLabel());
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetUnit(), acq2->GetAnalog(i)->GetUnit());
      TS_ASSERT_DELTA(acq->GetAnalog(i)->GetScale(), acq2->GetAnalog(i)->GetScale(), 5e-15);
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetOffset(), acq2->GetAnalog(i)->GetOffset());
      for(int j = 0 ; j < 500 ; j+=10)
      {
        TSM_ASSERT_DELTA("Channel #" + btk::ToString(i) + " - Sample " + btk::ToString(j), acq->GetAnalog(i)->GetValues()(j), acq2->GetAnalog(i)->GetValues()(j), 5e-11);
      }
    }
  };
  
  CXXTEST_TEST(rewrite_analog_gain)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,10,3,1);
    acq->SetPointFrequency(200.0);
    btk::Analog::Pointer analog0 = acq->GetAnalog(0);
    analog0->SetLabel("Test#1");
    analog0->SetDescription("It seems");
    analog0->SetGain(btk::Analog::PlusMinus10);
    analog0->SetOffset(123.0);
    analog0->SetScale(1.2345);
    analog0->SetUnit("N");
    btk::Analog::Pointer analog1 = acq->GetAnalog(1);
    analog1->SetLabel("Test#2");
    analog1->SetDescription("that BTK doesn't");
    analog1->SetGain(btk::Analog::PlusMinus5);
    analog1->SetOffset(456.0);
    analog1->SetScale(5.2341);
    analog1->SetUnit("Nmm");
    btk::Analog::Pointer analog2 = acq->GetAnalog(2);
    analog2->SetLabel("Test#3");
    analog2->SetDescription("write all properties");
    analog2->SetGain(btk::Analog::PlusMinus2Dot5);
    analog2->SetOffset(789.0);
    analog2->SetScale(10.025);
    analog2->SetUnit("Foo");
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(C3DFilePathOUT + "rewrite_analog_gain.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathOUT + "rewrite_analog_gain.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq2 = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    
    btk::Analog::Pointer analog0_ = acq2->GetAnalog(0);
    TS_ASSERT_EQUALS(analog0->GetLabel(), analog0_->GetLabel());
    TS_ASSERT_EQUALS(analog0->GetDescription(), analog0_->GetDescription());
    TS_ASSERT_EQUALS(analog0->GetGain(), analog0_->GetGain());
    TS_ASSERT_EQUALS(analog0->GetOffset(), analog0_->GetOffset());
    TS_ASSERT_DELTA(analog0->GetScale(), analog0_->GetScale(), 1e-5);
    TS_ASSERT_EQUALS(analog0->GetUnit(), analog0_->GetUnit());
    
    btk::Analog::Pointer analog1_ = acq2->GetAnalog(1);
    TS_ASSERT_EQUALS(analog1->GetLabel(), analog1_->GetLabel());
    TS_ASSERT_EQUALS(analog1->GetDescription(), analog1_->GetDescription());
    TS_ASSERT_EQUALS(analog1->GetGain(), analog1_->GetGain());
    TS_ASSERT_EQUALS(analog1->GetOffset(), analog1_->GetOffset());
    TS_ASSERT_DELTA(analog1->GetScale(), analog1_->GetScale(), 1e-5);
    TS_ASSERT_EQUALS(analog1->GetUnit(), analog1_->GetUnit());
    
    btk::Analog::Pointer analog2_ = acq2->GetAnalog(2);
    TS_ASSERT_EQUALS(analog2->GetLabel(), analog2_->GetLabel());
    TS_ASSERT_EQUALS(analog2->GetDescription(), analog2_->GetDescription());
    TS_ASSERT_EQUALS(analog2->GetGain(), analog2_->GetGain());
    TS_ASSERT_EQUALS(analog2->GetOffset(), analog2_->GetOffset());
    TS_ASSERT_DELTA(analog2->GetScale(), analog2_->GetScale(), 1e-5);
    TS_ASSERT_EQUALS(analog2->GetUnit(), analog2_->GetUnit());
  };
  
  CXXTEST_TEST(test_genscale_unmodified)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    writer->SetAcquisitionIO(reader->GetAcquisitionIO());
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    reader2->Update();

    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    for (int i = 0 ; i < acq->GetAnalogNumber() ; ++i)
    {
      TS_ASSERT_DELTA(acq2->GetAnalog(i)->GetScale(), acq->GetAnalog(i)->GetScale(), 1e-11);
      TS_ASSERT_EQUALS(acq2->GetAnalog(i)->GetOffset(), acq->GetAnalog(i)->GetOffset());
    }
    
    double genscale1 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("GEN_SCALE")->GetInfo()->ToDouble(0);
    double genscale2 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("GEN_SCALE")->GetInfo()->ToDouble(0);
    std::vector<double> scale1 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("SCALE")->GetInfo()->ToDouble();
    std::vector<double> scale2 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("SCALE")->GetInfo()->ToDouble();
    
    TS_ASSERT_EQUALS(scale1.size(), scale2.size());
    TS_ASSERT_DELTA(genscale1, genscale2, 1e-5);
    for (size_t i = 0 ; i < scale1.size() ; ++i)
    {
      TS_ASSERT_DELTA(scale1[i], scale2[i], 1e-5);
      TS_ASSERT_DELTA(scale1[i]*genscale1, scale2[i]*genscale2, 1e-11);
    }
  }
  
  CXXTEST_TEST(test_genscale_modified)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample09/PlugInC3D.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    acq->Update();
    acq->GetAnalog(4)->SetScale(514.8e-10);
    reader.reset();
    writer->SetInput(acq);
    writer->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample09_PlugInC3D.c3d");
    reader2->Update();

    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    for (int i = 0 ; i < acq->GetAnalogNumber() ; ++i)
    {
      TS_ASSERT_DELTA(acq2->GetAnalog(i)->GetScale(), acq->GetAnalog(i)->GetScale(), 1e-5);
      TS_ASSERT_EQUALS(acq2->GetAnalog(i)->GetOffset(), acq->GetAnalog(i)->GetOffset());
    }
    
    double genscale1 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("GEN_SCALE")->GetInfo()->ToDouble(0);
    double genscale2 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("GEN_SCALE")->GetInfo()->ToDouble(0);
    std::vector<double> scale1 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("SCALE")->GetInfo()->ToDouble();
    std::vector<double> scale2 = acq2->GetMetaData()->GetChild("ANALOG")->GetChild("SCALE")->GetInfo()->ToDouble();
    
    TS_ASSERT_EQUALS(scale1.size(), scale2.size());
    TS_ASSERT_DELTA(genscale2, 1e-5, 1e-6);
    for (size_t i = 0 ; i < scale1.size() ; ++i)
      TS_ASSERT_DELTA(scale1[i]*genscale1, scale2[i]*genscale2, 1e-11);
  }

  CXXTEST_TEST(UTF8)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample19/sample19.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "Я могу есть стекло, оно мне не вредит.c3d");
    writer->Update();
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "Я могу есть стекло, оно мне не вредит.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
  };
  
  CXXTEST_TEST(InternalsUpdateDefault)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/PiGMotion-FlatFoot-Full.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    reader.reset();
    // To test the CompatibleVicon option
    for (btk::Acquisition::PointConstIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
      (*it)->SetDescription("");
    for (btk::Acquisition::AnalogConstIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
      (*it)->SetDescription("");
    btk::MetaData::Pointer metadata = acq->GetMetaData()->GetChild("ANALYSIS");
    int num = metadata->GetChild("USED")->GetInfo()->ToInt(0);
    std::vector<std::string> blank(num);
    std::vector<uint8_t> dims = std::vector<uint8_t>(2,(int8_t)0); dims[1] = num;
    metadata->GetChild("DESCRIPTIONS")->GetInfo()->SetValues(dims, blank);
    metadata->GetChild("SUBJECTS")->GetInfo()->SetValues(dims, blank);
    // To test the DataBasedUpdate option
    int inc = 1;
    for (btk::Acquisition::PointIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
      (*it)->SetLabel("MyPoint#" + btk::ToString(inc++));
    inc = 1;
    for (btk::Acquisition::AnalogIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
      (*it)->SetLabel("MyAnalog#" + btk::ToString(inc++));
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(C3DFilePathOUT + "InternalsUpdateDefault.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "InternalsUpdateDefault.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    inc = 1;
    for (btk::Acquisition::PointIterator it = acq2->BeginPoint() ; it != acq2->EndPoint() ; ++it)
      TS_ASSERT_EQUALS((*it)->GetLabel(), "MyPoint#" + btk::ToString(inc++));
    inc = 1;
    for (btk::Acquisition::AnalogIterator it = acq2->BeginAnalog() ; it != acq2->EndAnalog() ; ++it)
      TS_ASSERT_EQUALS((*it)->GetLabel(), "MyAnalog#" + btk::ToString(inc++));
    
    metadata = acq2->GetMetaData()->GetChild("POINT");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 4);
    for (int i = 0 ; i < acq2->GetPointNumber() ; ++i)
      TS_ASSERT_EQUALS(blank[i], "    ");
    
    metadata = acq2->GetMetaData()->GetChild("ANALOG");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 4);
    for (int i = 0 ; i < acq2->GetAnalogNumber() ; ++i)
      TS_ASSERT_EQUALS(blank[i], "    ");
    
    metadata = acq2->GetMetaData()->GetChild("ANALYSIS");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 4);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "    ");
    dims = metadata->GetChild("SUBJECTS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("SUBJECTS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 4);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "    ");
  };
  
  CXXTEST_TEST(InternalsUpdateDataBased)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/PiGMotion-FlatFoot-Full.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    reader.reset();
    // To test the *non use* of the CompatibleVicon option
    for (btk::Acquisition::PointConstIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
      (*it)->SetDescription("");
    for (btk::Acquisition::AnalogConstIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
      (*it)->SetDescription("");
    btk::MetaData::Pointer metadata = acq->GetMetaData()->GetChild("ANALYSIS");
    int num = metadata->GetChild("USED")->GetInfo()->ToInt(0);
    std::vector<std::string> blank(num);
    std::vector<uint8_t> dims = std::vector<uint8_t>(2,(int8_t)0); dims[1] = num;
    metadata->GetChild("DESCRIPTIONS")->GetInfo()->SetValues(dims, blank);
    metadata->GetChild("SUBJECTS")->GetInfo()->SetValues(dims, blank);
    // To test the DataBasedUpdate option
    int inc = 1;
    for (btk::Acquisition::PointIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
      (*it)->SetLabel("MyPoint#" + btk::ToString(inc++));
    inc = 1;
    for (btk::Acquisition::AnalogIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
      (*it)->SetLabel("MyAnalog#" + btk::ToString(inc++));
    
    btk::C3DFileIO::Pointer io = btk::C3DFileIO::New();
    io->SetInternalsUpdateOptions(btk::C3DFileIO::DataBasedUpdate);
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetAcquisitionIO(io);
    writer->SetFilename(C3DFilePathOUT + "InternalsUpdateDataBased.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "InternalsUpdateDataBased.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    inc = 1;
    for (btk::Acquisition::PointIterator it = acq2->BeginPoint() ; it != acq2->EndPoint() ; ++it)
      TS_ASSERT_EQUALS((*it)->GetLabel(), "MyPoint#" + btk::ToString(inc++));
    inc = 1;
    for (btk::Acquisition::AnalogIterator it = acq2->BeginAnalog() ; it != acq2->EndAnalog() ; ++it)
      TS_ASSERT_EQUALS((*it)->GetLabel(), "MyAnalog#" + btk::ToString(inc++));
    
    metadata = acq2->GetMetaData()->GetChild("POINT");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 0);
    for (int i = 0 ; i < acq2->GetPointNumber() ; ++i)
      TS_ASSERT_EQUALS(blank[i], "");
    
    metadata = acq2->GetMetaData()->GetChild("ANALOG");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 0);
    for (int i = 0 ; i < acq2->GetAnalogNumber() ; ++i)
      TS_ASSERT_EQUALS(blank[i], "");
    
    metadata = acq2->GetMetaData()->GetChild("ANALYSIS");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 0);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "");
    dims = metadata->GetChild("SUBJECTS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("SUBJECTS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 0);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "");
  };
  
  CXXTEST_TEST(InternalsUpdateViconCompatibleOnly)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/PiGMotion-FlatFoot-Full.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    reader.reset();
    // To test the use of the CompatibleVicon option
    for (btk::Acquisition::PointConstIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
      (*it)->SetDescription("");
    for (btk::Acquisition::AnalogConstIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
      (*it)->SetDescription("");
    btk::MetaData::Pointer metadata = acq->GetMetaData()->GetChild("ANALYSIS");
    int num = metadata->GetChild("USED")->GetInfo()->ToInt(0);
    std::vector<std::string> blank(num);
    std::vector<uint8_t> dims = std::vector<uint8_t>(2,(int8_t)0); dims[1] = num;
    metadata->GetChild("DESCRIPTIONS")->GetInfo()->SetValues(dims, blank);
    metadata->GetChild("SUBJECTS")->GetInfo()->SetValues(dims, blank);
    // To test the *non use* of the DataBasedUpdate option
    int inc = 1;
    for (btk::Acquisition::PointIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
    {  
      (*it)->SetLabel("MyPoint#" + btk::ToString(inc++));
      (*it)->SetDescription("test");
    }
    inc = 1;
    for (btk::Acquisition::AnalogIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
    {
      (*it)->SetLabel("MyAnalog#" + btk::ToString(inc++));
      (*it)->SetDescription("test");
    }
    
    btk::C3DFileIO::Pointer io = btk::C3DFileIO::New();
    io->SetInternalsUpdateOptions(btk::C3DFileIO::CompatibleVicon);
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetAcquisitionIO(io);
    writer->SetFilename(C3DFilePathOUT + "InternalsUpdateViconCompatibleOnly.c3d");
    writer->Update();
    
    reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/PiGMotion-FlatFoot-Full.c3d");
    reader->Update();
    acq = reader->GetOutput();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "InternalsUpdateViconCompatibleOnly.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    inc = 0;
    for (btk::Acquisition::PointIterator it = acq2->BeginPoint() ; it != acq2->EndPoint() ; ++it)
    {
      TS_ASSERT_EQUALS((*it)->GetLabel(), acq->GetPoint(inc)->GetLabel());
      TS_ASSERT_EQUALS((*it)->GetDescription(), acq->GetPoint(inc)->GetDescription());
      ++inc;
    }
    inc = 0;
    for (btk::Acquisition::AnalogIterator it = acq2->BeginAnalog() ; it != acq2->EndAnalog() ; ++it)
    {
      TS_ASSERT_EQUALS((*it)->GetLabel(), acq->GetAnalog(inc)->GetLabel());
      TS_ASSERT_EQUALS((*it)->GetDescription(), acq->GetAnalog(inc)->GetDescription());
      ++inc;
    }

    metadata = acq2->GetMetaData()->GetChild("ANALYSIS");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 4);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "    ");
    dims = metadata->GetChild("SUBJECTS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("SUBJECTS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 4);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "    ");
  };
  
  CXXTEST_TEST(InternalsUpdateUpdateMetaDataBased)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/PiGMotion-FlatFoot-Full.c3d");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    reader.reset();
    // To test the *non use* of the CompatibleVicon option
    for (btk::Acquisition::PointConstIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
      (*it)->SetDescription("");
    for (btk::Acquisition::AnalogConstIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
      (*it)->SetDescription("");
    btk::MetaData::Pointer metadata = acq->GetMetaData()->GetChild("ANALYSIS");
    int num = metadata->GetChild("USED")->GetInfo()->ToInt(0);
    std::vector<std::string> blank(num);
    std::vector<uint8_t> dims = std::vector<uint8_t>(2,(int8_t)0); dims[1] = num;
    metadata->GetChild("DESCRIPTIONS")->GetInfo()->SetValues(dims, blank);
    metadata->GetChild("SUBJECTS")->GetInfo()->SetValues(dims, blank);
    // To test the *non use* of the DataBasedUpdate option
    int inc = 1;
    for (btk::Acquisition::PointIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
    {  
      (*it)->SetLabel("MyPoint#" + btk::ToString(inc++));
      (*it)->SetDescription("test");
    }
    inc = 1;
    for (btk::Acquisition::AnalogIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
    {
      (*it)->SetLabel("MyAnalog#" + btk::ToString(inc++));
      (*it)->SetDescription("test");
      (*it)->SetScale(1.0);
      (*it)->SetOffset(0.0);
    }
    acq->GetMetaData()->GetChild("POINT")->GetChild("RATE")->GetInfo()->SetValues(400.0f);
      
    btk::C3DFileIO::Pointer io = btk::C3DFileIO::New();
    io->SetInternalsUpdateOptions(btk::C3DFileIO::MetaDataBasedUpdate);
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetAcquisitionIO(io);
    writer->SetFilename(C3DFilePathOUT + "InternalsUpdateUpdateMetaDataBased.c3d");
    writer->Update();
    
    reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/PiGMotion-FlatFoot-Full.c3d");
    reader->Update();
    acq = reader->GetOutput();
    
    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "InternalsUpdateUpdateMetaDataBased.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), acq2->GetFirstFrame());
    TS_ASSERT_EQUALS(400.0, acq2->GetPointFrequency());
    TS_ASSERT_EQUALS(acq->GetPointNumber(), acq2->GetPointNumber());
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), acq2->GetPointFrameNumber());
    TS_ASSERT_EQUALS(4000.0, acq2->GetAnalogFrequency());
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), acq2->GetAnalogNumber());
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), acq2->GetAnalogFrameNumber());
    TS_ASSERT_EQUALS(acq->GetPointUnit(), acq2->GetPointUnit());
    
    inc = 0;
    for (btk::Acquisition::PointIterator it = acq2->BeginPoint() ; it != acq2->EndPoint() ; ++it)
    {
      TS_ASSERT_EQUALS((*it)->GetLabel(), acq->GetPoint(inc)->GetLabel());
      TS_ASSERT_EQUALS((*it)->GetDescription(), acq->GetPoint(inc)->GetDescription());
      ++inc;
    }
    inc = 0;
    for (btk::Acquisition::AnalogIterator it = acq2->BeginAnalog() ; it != acq2->EndAnalog() ; ++it)
    {
      TS_ASSERT_EQUALS((*it)->GetLabel(), acq->GetAnalog(inc)->GetLabel());
      TS_ASSERT_EQUALS((*it)->GetDescription(), acq->GetAnalog(inc)->GetDescription());
      TS_ASSERT_EQUALS((*it)->GetScale(), acq->GetAnalog(inc)->GetScale());
      TS_ASSERT_EQUALS((*it)->GetOffset(), acq->GetAnalog(inc)->GetOffset());
      ++inc;
    }

    metadata = acq2->GetMetaData()->GetChild("ANALYSIS");
    dims = metadata->GetChild("DESCRIPTIONS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("DESCRIPTIONS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 0);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "");
    dims = metadata->GetChild("SUBJECTS")->GetInfo()->GetDimensions();
    blank = metadata->GetChild("SUBJECTS")->GetInfo()->ToString();
    TS_ASSERT_EQUALS(dims[0], 0);
    for (int i = 0 ; i < num ; ++i)
      TS_ASSERT_EQUALS(blank[i], "");
  };
  
  CXXTEST_TEST(InternalsUpdateUpdateMetaDataBased_EventsHeader)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "sample01/Eb015pi.c3d");
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    btk::C3DFileIO::Pointer io = static_pointer_cast<btk::C3DFileIO>(reader->GetAcquisitionIO());
    io->SetInternalsUpdateOptions(btk::C3DFileIO::MetaDataBasedUpdate);
    writer->SetAcquisitionIO(io);
    writer->SetInput(reader->GetOutput());
    writer->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_metadata.c3d");
    writer->Update();

    btk::AcquisitionFileReader::Pointer reader2 = btk::AcquisitionFileReader::New();
    reader2->SetFilename(C3DFilePathOUT + "sample01_Eb015pi_metadata.c3d");
    reader2->Update();
    btk::Acquisition::Pointer acq2 = reader2->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetEventNumber(), acq2->GetEventNumber());
    for (int i = 0 ; i < 3 ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetEvent(i)->GetLabel(), acq2->GetEvent(i)->GetLabel());
      TS_ASSERT_DELTA(acq->GetEvent(i)->GetTime(), acq2->GetEvent(i)->GetTime(), 1e-5);
    }
    TS_ASSERT(acq->GetMetaData()->FindChild("EVENT") == acq->EndMetaData());
  };
  
  CXXTEST_TEST(AnalogOffsetStoredAsReal_12Bits)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,10,1);
    acq->SetPointFrequency(50.0);
    acq->GetAnalog(0)->SetOffset(-1.1);
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(C3DFilePathOUT + "AnalogOffsetStoredAsReal_12.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathOUT + "AnalogOffsetStoredAsReal_12.c3d");
    reader->Update();
    acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("ANALOG")->GetChild("OFFSET")->GetInfo()->GetFormat(), btk::MetaDataInfo::Real);
    
    TS_ASSERT(acq->GetAnalog(0)->GetValues().cwiseAbs().maxCoeff() <= 1e-5);
  };
  
  CXXTEST_TEST(AnalogOffsetStoredAsReal_16Bits)
  {
    btk::Acquisition::Pointer acq = btk::Acquisition::New();
    acq->Init(0,10,1);
    acq->SetAnalogResolution(btk::Acquisition::Bit16);
    acq->SetPointFrequency(50.0);
    acq->GetAnalog(0)->SetOffset(-1.1);
    
    btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
    writer->SetInput(acq);
    writer->SetFilename(C3DFilePathOUT + "AnalogOffsetStoredAsReal_16.c3d");
    writer->Update();
    
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathOUT + "AnalogOffsetStoredAsReal_16.c3d");
    reader->Update();
    acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetMetaData()->GetChild("ANALOG")->GetChild("OFFSET")->GetInfo()->GetFormat(), btk::MetaDataInfo::Real);
    
    TS_ASSERT(acq->GetAnalog(0)->GetValues().cwiseAbs().maxCoeff() <= 1e-5);
  };
};

CXXTEST_SUITE_REGISTRATION(C3DFileWriterTest)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, NoFileNoInput)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, NoFile)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, NoFileWithIO)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pi_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pr_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015sr_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vi_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_clearAnalog)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_clearPoint)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_clearAll)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pi_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vi_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_from_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pi_from_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vi_from_Eb015pr) 
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015si_from_Eb015pi) 
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_from_Eb015pi)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015vr_from_Eb015sr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015sr_from_Eb015pr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample01_Eb015pr_from_Eb015vr)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample09_PlugInC3D_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, sample19_sample19_rewrited)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, emptyAcquisition_Template)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, convertTRC2C3D)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, noInputModification)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, acq100000)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, convertDelsysEMG2C3D)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, convertCLB2C3D)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, rewrite_analog_gain)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, test_genscale_unmodified)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, test_genscale_modified)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, UTF8)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, InternalsUpdateDefault)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, InternalsUpdateDataBased)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, InternalsUpdateViconCompatibleOnly)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, InternalsUpdateUpdateMetaDataBased)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, InternalsUpdateUpdateMetaDataBased_EventsHeader)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, AnalogOffsetStoredAsReal_12Bits)
CXXTEST_TEST_REGISTRATION(C3DFileWriterTest, AnalogOffsetStoredAsReal_16Bits)
#endif
