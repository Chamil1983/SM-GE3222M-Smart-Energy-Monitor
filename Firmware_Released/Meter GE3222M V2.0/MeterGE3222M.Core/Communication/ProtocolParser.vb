Option Strict On

' SM-GE3222M V2.0 - Protocol Parser Base
' Auto-detects V1 (Tag:Value) or V2 (JSON) protocol and delegates to appropriate parser

Imports MeterGE3222M.Core.Models

Namespace Communication
    ''' <summary>
    ''' Protocol version enumeration
    ''' </summary>
    Public Enum ProtocolVersion
        Unknown = 0
        V1_TagValue = 1
        V2_Json = 2
    End Enum
    
    ''' <summary>
    ''' Base protocol parser with auto-detection
    ''' Detects protocol version and delegates to appropriate parser
    ''' </summary>
    Public Class ProtocolParser
        Private ReadOnly _v1Parser As ProtocolV1Parser
        Private ReadOnly _v2Parser As ProtocolV2Parser
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        Public Sub New()
            _v1Parser = New ProtocolV1Parser()
            _v2Parser = New ProtocolV2Parser()
        End Sub
        
        ''' <summary>
        ''' Auto-detect protocol version from raw data
        ''' </summary>
        ''' <param name="data">Raw text data from meter</param>
        ''' <returns>Detected protocol version</returns>
        Public Shared Function DetectProtocol(data As String) As ProtocolVersion
            If String.IsNullOrWhiteSpace(data) Then
                Return ProtocolVersion.Unknown
            End If
            
            Dim trimmedData = data.TrimStart()
            
            ' V2 JSON: Starts with '{'
            If trimmedData.StartsWith("{") Then
                Return ProtocolVersion.V2_Json
            End If
            
            ' V1 Tag:Value: Contains tags like AE1, AP1, BE1, etc.
            If trimmedData.Contains("AE1") OrElse 
               trimmedData.Contains("AP1") OrElse 
               trimmedData.Contains("BE1") OrElse 
               trimmedData.Contains("BP1") OrElse 
               trimmedData.Contains("CE1") OrElse 
               trimmedData.Contains("CP1") OrElse 
               trimmedData.Contains("TE1") OrElse 
               trimmedData.Contains("TP1") Then
                Return ProtocolVersion.V1_TagValue
            End If
            
            Return ProtocolVersion.Unknown
        End Function
        
        ''' <summary>
        ''' Parse raw data with auto-detection
        ''' </summary>
        ''' <param name="data">Raw text data from meter</param>
        ''' <returns>Parsed MeterSnapshot</returns>
        Public Function Parse(data As String) As MeterSnapshot
            Dim version = DetectProtocol(data)
            
            Select Case version
                Case ProtocolVersion.V1_TagValue
                    Return _v1Parser.Parse(data)
                    
                Case ProtocolVersion.V2_Json
                    Return _v2Parser.Parse(data)
                    
                Case Else
                    ' Return invalid snapshot
                    Return New MeterSnapshot() With {.Valid = False}
            End Select
        End Function
        
        ''' <summary>
        ''' Try parse raw data and return success status
        ''' </summary>
        ''' <param name="data">Raw text data from meter</param>
        ''' <param name="snapshot">Parsed MeterSnapshot (output)</param>
        ''' <returns>True if parsed successfully</returns>
        Public Function TryParse(data As String, ByRef snapshot As MeterSnapshot) As Boolean
            Try
                snapshot = Parse(data)
                Return snapshot IsNot Nothing AndAlso snapshot.Valid
            Catch ex As Exception
                snapshot = New MeterSnapshot() With {.Valid = False}
                Return False
            End Try
        End Function
        
        ''' <summary>
        ''' Get V1 parser instance
        ''' </summary>
        Public ReadOnly Property V1Parser As ProtocolV1Parser
            Get
                Return _v1Parser
            End Get
        End Property
        
        ''' <summary>
        ''' Get V2 parser instance
        ''' </summary>
        Public ReadOnly Property V2Parser As ProtocolV2Parser
            Get
                Return _v2Parser
            End Get
        End Property
    End Class
End Namespace
