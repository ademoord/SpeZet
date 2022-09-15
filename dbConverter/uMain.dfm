object fmMain: TfmMain
  Left = 0
  Top = 0
  Caption = 'fmMain'
  ClientHeight = 578
  ClientWidth = 665
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object mmForm: TMemo
    Left = 0
    Top = 72
    Width = 665
    Height = 506
    Align = alBottom
    Lines.Strings = (
      'mmForm')
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object buJob: TButton
    Left = 24
    Top = 25
    Width = 75
    Height = 25
    Caption = 'buJob'
    TabOrder = 1
    OnClick = buJobClick
  end
  object buEmployee: TButton
    Left = 105
    Top = 25
    Width = 75
    Height = 25
    Caption = 'buEmployee'
    TabOrder = 2
    OnClick = buEmployeeClick
  end
  object buOwner: TButton
    Left = 191
    Top = 25
    Width = 75
    Height = 25
    Caption = 'buOwner'
    TabOrder = 3
    OnClick = buOwnerClick
  end
  object buOrder: TButton
    Left = 272
    Top = 25
    Width = 75
    Height = 25
    Caption = 'buOrder'
    TabOrder = 4
    OnClick = buOrderClick
  end
  object buRate: TButton
    Left = 353
    Top = 25
    Width = 75
    Height = 25
    Caption = 'buRate'
    TabOrder = 5
    OnClick = buRateClick
  end
  object buEmployeeJob: TButton
    Left = 440
    Top = 25
    Width = 105
    Height = 25
    Caption = 'buEmployeeJob'
    TabOrder = 6
    OnClick = buEmployeeJobClick
  end
  object dbMySQL: TSQLConnection
    ConnectionName = 'MYSQLCONNECTION'
    DriverName = 'MySQL'
    GetDriverFunc = 'getSQLDriverMYSQL'
    LibraryName = 'dbxmys.dll'
    LoginPrompt = False
    Params.Strings = (
      'DriverUnit=DBXMySQL'
      
        'DriverPackageLoader=TDBXDynalinkDriverLoader,DbxCommonDriver150.' +
        'bpl'
      
        'DriverAssemblyLoader=Borland.Data.TDBXDynalinkDriverLoader,Borla' +
        'nd.Data.DbxCommonDriver,Version=15.0.0.0,Culture=neutral,PublicK' +
        'eyToken=91d62ebb5b0d1b1b'
      
        'MetaDataPackageLoader=TDBXMySqlMetaDataCommandFactory,DbxMySQLDr' +
        'iver150.bpl'
      
        'MetaDataAssemblyLoader=Borland.Data.TDBXMySqlMetaDataCommandFact' +
        'ory,Borland.Data.DbxMySQLDriver,Version=15.0.0.0,Culture=neutral' +
        ',PublicKeyToken=91d62ebb5b0d1b1b'
      'GetDriverFunc=getSQLDriverMYSQL'
      'LibraryName=dbxmys.dll'
      'VendorLib=LIBMYSQL.dll'
      'MaxBlobSize=-1'
      'DriverName=MySQL'
      'HostName='
      'Database='
      'User_Name='
      'Password='
      'ServerCharSet='
      'BlobSize=-1'
      'ErrorResourceFile='
      'LocaleCode=0000'
      'Compressed=False'
      'Encrypted=False'
      'ConnectTimeout=60')
    VendorLib = 'LIBMYSQL.dll'
    Left = 128
    Top = 80
  end
  object qryMySQL: TSQLDataSet
    DbxCommandType = 'Dbx.SQL'
    MaxBlobSize = -1
    Params = <>
    SQLConnection = dbMySQL
    Left = 128
    Top = 136
  end
  object qryPdox: TQuery
    Left = 48
    Top = 80
  end
  object qryLookup: TSQLDataSet
    DbxCommandType = 'Dbx.SQL'
    Params = <>
    Left = 128
    Top = 192
  end
end
