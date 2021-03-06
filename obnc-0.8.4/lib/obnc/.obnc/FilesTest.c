/*GENERATED BY OBNC 0.8.4*/

#include "/home/gunter/GM_LANGUAGES/OBERON/V_07/OBNC/obnc-0.8.4/lib/obnc/OBNC_.h"
#include "Files.h"

static void TestOld_(void)
{
	Files_File_ f_ = 0;

	f_ = Files_Old_("FilesTest.obn", 14);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 27);
	OBNC_ASSERT(((void) OBNC_PT(f_), OBNC_IS(OBNC_TD(f_, struct Files_Handle_Heap), &Files_Handle_id, 0)), "FilesTest.obn", 29);
}


static void TestNew_(void)
{
	Files_File_ f_ = 0;

	f_ = Files_New_("NewTest", 8);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 36);
	OBNC_ASSERT(((void) OBNC_PT(f_), OBNC_IS(OBNC_TD(f_, struct Files_Handle_Heap), &Files_Handle_id, 0)), "FilesTest.obn", 38);
}


static void TestRegister_(void);

static int TestRegister_IsRider_Local(Files_Rider_ *r_, const OBNC_Td *r_td)
{
	return OBNC_IS(r_td, &Files_Rider_id, 0);
}


static void TestRegister_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	int res_;

	f_ = Files_New_("RegisterTest", 13);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 51);
	OBNC_ASSERT(TestRegister_IsRider_Local(&r_, &Files_Rider_td), "FilesTest.obn", 52);
	Files_Register_(f_);
	f_ = Files_Old_("RegisterTest", 13);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 56);
	Files_Delete_("RegisterTest", 13, &res_);
	OBNC_ASSERT(res_ == 0, "FilesTest.obn", 58);
	f_ = Files_New_("RegisterTest", 13);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 61);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_Write_(&r_, &Files_Rider_td, (unsigned char) 37);
	Files_Close_(f_);
	Files_Register_(f_);
	OBNC_ASSERT(Files_Length_(f_) == 1, "FilesTest.obn", 66);
	f_ = Files_Old_("RegisterTest", 13);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 69);
	OBNC_ASSERT(Files_Length_(f_) == 1, "FilesTest.obn", 70);
	Files_Delete_("RegisterTest", 13, &res_);
	OBNC_ASSERT(res_ == 0, "FilesTest.obn", 73);
}


static void TestClose_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};

	f_ = Files_New_("CloseTest", 10);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 81);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_Write_(&r_, &Files_Rider_td, (unsigned char) 65);
	OBNC_ASSERT(Files_Length_(f_) <= 1, "FilesTest.obn", 84);
	Files_Close_(f_);
	OBNC_ASSERT(Files_Length_(f_) == 1, "FilesTest.obn", 87);
}


static void TestPurge_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};

	f_ = Files_New_("PurgeTest", 10);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 95);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_Write_(&r_, &Files_Rider_td, (unsigned char) 65);
	Files_Purge_(f_);
	OBNC_ASSERT(Files_Length_(f_) == 0, "FilesTest.obn", 100);
}


static void TestDelete_(void)
{
	Files_File_ f_ = 0;
	int res_;

	f_ = Files_New_("DeleteTest", 11);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 108);
	Files_Register_(f_);
	f_ = Files_Old_("DeleteTest", 11);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 111);
	Files_Delete_("DeleteTest", 11, &res_);
	OBNC_ASSERT(res_ == 0, "FilesTest.obn", 113);
	f_ = Files_Old_("DeleteTest", 11);
	OBNC_ASSERT(f_ == 0, "FilesTest.obn", 116);
}


static void TestRename_(void)
{
	Files_File_ f_ = 0;
	int res_;

	f_ = Files_New_("RenameTest", 11);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 124);
	Files_Register_(f_);
	f_ = Files_Old_("RenameTest", 11);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 127);
	Files_Rename_("RenameTest", 11, "RenameTest1", 12, &res_);
	OBNC_ASSERT(res_ == 0, "FilesTest.obn", 130);
	f_ = Files_Old_("RenameTest", 11);
	OBNC_ASSERT(f_ == 0, "FilesTest.obn", 133);
	f_ = Files_Old_("RenameTest1", 12);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 135);
	Files_Delete_("RenameTest1", 12, &res_);
	OBNC_ASSERT(res_ == 0, "FilesTest.obn", 139);
}


static void TestLength_(void)
{
	Files_File_ f_ = 0;
	int res_;

	f_ = Files_New_("LengthTest", 11);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 147);
	Files_Register_(f_);
	OBNC_ASSERT(Files_Length_(f_) == 0, "FilesTest.obn", 149);
	Files_Delete_("LengthTest", 11, &res_);
	OBNC_ASSERT(res_ == 0, "FilesTest.obn", 152);
}


static void TestDate_(void)
{
	Files_File_ f_ = 0;
	int t_, d_;
	int hour_, minute_, second_, year_, month_, day_;

	f_ = Files_Old_("FilesTest.obn", 14);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 161);
	Files_GetDate_(f_, &t_, &d_);
	hour_ = (OBNC_DIV(t_, 4096));
	OBNC_ASSERT(hour_ >= 0, "FilesTest.obn", 164);
	OBNC_ASSERT(hour_ < 24, "FilesTest.obn", 165);
	minute_ = (OBNC_MOD(OBNC_DIV(t_, 64), 64));
	OBNC_ASSERT(minute_ >= 0, "FilesTest.obn", 167);
	OBNC_ASSERT(minute_ < 60, "FilesTest.obn", 168);
	second_ = (OBNC_MOD(t_, 64));
	OBNC_ASSERT(second_ >= 0, "FilesTest.obn", 170);
	OBNC_ASSERT(second_ < 60, "FilesTest.obn", 171);
	year_ = (OBNC_DIV(d_, 512));
	OBNC_ASSERT(year_ >= 0, "FilesTest.obn", 173);
	month_ = (OBNC_MOD(OBNC_DIV(d_, 32), 16));
	OBNC_ASSERT(month_ >= 1, "FilesTest.obn", 175);
	OBNC_ASSERT(month_ <= 12, "FilesTest.obn", 176);
	day_ = (OBNC_MOD(d_, 32));
	OBNC_ASSERT(day_ >= 1, "FilesTest.obn", 178);
	OBNC_ASSERT(day_ <= 31, "FilesTest.obn", 179);
}


static void TestSet_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};

	f_ = Files_New_("SetTest", 8);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 188);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 191);
}


static void TestPos_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};

	f_ = Files_New_("PosTest", 8);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 199);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	OBNC_ASSERT(Files_Pos_(&r_, &Files_Rider_td) == 0, "FilesTest.obn", 202);
}


static void TestBase_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};

	f_ = Files_New_("BaseTest", 9);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 210);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	OBNC_ASSERT(Files_Base_(&r_, &Files_Rider_td) == f_, "FilesTest.obn", 213);
}


static void TestReadWrite_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	unsigned char b_;

	f_ = Files_New_("ReadWriteTest", 14);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 222);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_Write_(&r_, &Files_Rider_td, (unsigned char) 65);
	Files_Close_(f_);
	OBNC_ASSERT(Files_Length_(f_) == 1, "FilesTest.obn", 226);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_Read_(&r_, &Files_Rider_td, &b_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 229);
	OBNC_ASSERT(b_ == 65, "FilesTest.obn", 230);
	OBNC_ASSERT(Files_Pos_(&r_, &Files_Rider_td) == 1, "FilesTest.obn", 232);
}


static void TestReadWriteInt_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	int i_;

	f_ = Files_New_("ReadWriteIntTest", 17);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 241);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_WriteInt_(&r_, &Files_Rider_td, 100);
	Files_WriteInt_(&r_, &Files_Rider_td, -1000);
	Files_Close_(f_);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_ReadInt_(&r_, &Files_Rider_td, &i_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 250);
	OBNC_ASSERT(i_ == 100, "FilesTest.obn", 251);
	Files_ReadInt_(&r_, &Files_Rider_td, &i_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 253);
	OBNC_ASSERT(i_ == -1000, "FilesTest.obn", 254);
	Files_ReadInt_(&r_, &Files_Rider_td, &i_);
	OBNC_ASSERT(r_.eof_, "FilesTest.obn", 257);
}


static void TestReadWriteReal_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	double x_;

	f_ = Files_New_("ReadWriteRealTest", 18);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 266);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_WriteReal_(&r_, &Files_Rider_td, 3.14);
	Files_WriteReal_(&r_, &Files_Rider_td, -3.14);
	Files_Close_(f_);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_ReadReal_(&r_, &Files_Rider_td, &x_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 275);
	OBNC_ASSERT(x_ == 3.14, "FilesTest.obn", 276);
	Files_ReadReal_(&r_, &Files_Rider_td, &x_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 278);
	OBNC_ASSERT(x_ == -3.14, "FilesTest.obn", 279);
	Files_ReadReal_(&r_, &Files_Rider_td, &x_);
	OBNC_ASSERT(r_.eof_, "FilesTest.obn", 282);
}


static void TestReadWriteNum_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	int i_;

	f_ = Files_New_("ReadWriteNumTest", 17);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 291);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_WriteNum_(&r_, &Files_Rider_td, 100);
	Files_WriteNum_(&r_, &Files_Rider_td, -1000);
	Files_Close_(f_);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_ReadNum_(&r_, &Files_Rider_td, &i_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 300);
	OBNC_ASSERT(i_ == 100, "FilesTest.obn", 301);
	Files_ReadNum_(&r_, &Files_Rider_td, &i_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 303);
	OBNC_ASSERT(i_ == -1000, "FilesTest.obn", 304);
	Files_ReadNum_(&r_, &Files_Rider_td, &i_);
	OBNC_ASSERT(r_.eof_, "FilesTest.obn", 307);
}


static void TestReadWriteString_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	char s_[32];

	f_ = Files_New_("ReadWriteStringTest", 20);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 316);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_WriteString_(&r_, &Files_Rider_td, "hello", 6);
	Files_WriteString_(&r_, &Files_Rider_td, "there", 6);
	Files_WriteString_(&r_, &Files_Rider_td, "", 1);
	Files_Close_(f_);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_ReadString_(&r_, &Files_Rider_td, s_, 32);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 326);
	OBNC_ASSERT(OBNC_CMP(s_, 32, "hello", 6) == 0, "FilesTest.obn", 327);
	Files_ReadString_(&r_, &Files_Rider_td, s_, 32);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 329);
	OBNC_ASSERT(OBNC_CMP(s_, 32, "there", 6) == 0, "FilesTest.obn", 330);
	Files_ReadString_(&r_, &Files_Rider_td, s_, 32);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 332);
	OBNC_ASSERT(OBNC_CMP(s_, 32, "", 1) == 0, "FilesTest.obn", 333);
	Files_ReadString_(&r_, &Files_Rider_td, s_, 32);
	OBNC_ASSERT(r_.eof_, "FilesTest.obn", 336);
}


static void TestReadWriteSet_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	unsigned int s_;

	f_ = Files_New_("ReadWriteSetTest", 17);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 345);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_WriteSet_(&r_, &Files_Rider_td, 0x0u);
	Files_WriteSet_(&r_, &Files_Rider_td, 0x1u);
	Files_WriteSet_(&r_, &Files_Rider_td, 0x3u);
	Files_Close_(f_);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_ReadSet_(&r_, &Files_Rider_td, &s_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 355);
	OBNC_ASSERT(s_ == 0x0u, "FilesTest.obn", 356);
	Files_ReadSet_(&r_, &Files_Rider_td, &s_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 358);
	OBNC_ASSERT(s_ == 0x1u, "FilesTest.obn", 359);
	Files_ReadSet_(&r_, &Files_Rider_td, &s_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 361);
	OBNC_ASSERT(s_ == 0x3u, "FilesTest.obn", 362);
	Files_ReadSet_(&r_, &Files_Rider_td, &s_);
	OBNC_ASSERT(r_.eof_, "FilesTest.obn", 365);
}


static void TestReadWriteBool_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	int b_;

	f_ = Files_New_("ReadWriteBoolTest", 18);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 374);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_WriteBool_(&r_, &Files_Rider_td, 1);
	Files_WriteBool_(&r_, &Files_Rider_td, 0);
	Files_Close_(f_);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_ReadBool_(&r_, &Files_Rider_td, &b_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 383);
	OBNC_ASSERT(b_, "FilesTest.obn", 384);
	Files_ReadBool_(&r_, &Files_Rider_td, &b_);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 386);
	OBNC_ASSERT(! b_, "FilesTest.obn", 387);
	Files_ReadBool_(&r_, &Files_Rider_td, &b_);
	OBNC_ASSERT(r_.eof_, "FilesTest.obn", 390);
}


static void TestReadWriteBytes_(void)
{
	Files_File_ f_ = 0;
	Files_Rider_ r_ = {0};
	unsigned char buf_[4];
	int i_;

	for (i_ = 0; i_ <= 3; i_ += 1) {
		buf_[OBNC_IT(i_, 4)] = (unsigned char) (i_ + 1);
	}
	f_ = Files_New_("ReadWriteBytesTest", 19);
	OBNC_ASSERT(f_ != 0, "FilesTest.obn", 402);
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_WriteBytes_(&r_, &Files_Rider_td, buf_, 4, 4);
	Files_WriteBytes_(&r_, &Files_Rider_td, buf_, 4, 4);
	Files_Close_(f_);
	for (i_ = 0; i_ <= 3; i_ += 1) {
		buf_[OBNC_IT(i_, 4)] = (unsigned char) 0;
	}
	Files_Set_(&r_, &Files_Rider_td, f_, 0);
	Files_ReadBytes_(&r_, &Files_Rider_td, buf_, 4, 4);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 414);
	for (i_ = 0; i_ <= 3; i_ += 1) {
		OBNC_ASSERT(buf_[OBNC_IT(i_, 4)] == (i_ + 1), "FilesTest.obn", 417);
	}
	Files_ReadBytes_(&r_, &Files_Rider_td, buf_, 4, 4);
	OBNC_ASSERT(! r_.eof_, "FilesTest.obn", 420);
	for (i_ = 0; i_ <= 3; i_ += 1) {
		OBNC_ASSERT(buf_[OBNC_IT(i_, 4)] == (i_ + 1), "FilesTest.obn", 423);
	}
	Files_ReadBytes_(&r_, &Files_Rider_td, buf_, 4, 4);
	OBNC_ASSERT(r_.eof_, "FilesTest.obn", 427);
}


int main(int argc, char *argv[])
{
	OBNC_Initialize(argc, argv);
	Files_Init();
	TestOld_();
	TestNew_();
	TestRegister_();
	TestClose_();
	TestPurge_();
	TestDelete_();
	TestRename_();
	TestLength_();
	TestDate_();
	TestSet_();
	TestPos_();
	TestBase_();
	TestReadWrite_();
	TestReadWriteInt_();
	TestReadWriteReal_();
	TestReadWriteNum_();
	TestReadWriteString_();
	TestReadWriteSet_();
	TestReadWriteBool_();
	TestReadWriteBytes_();
	return 0;
}
