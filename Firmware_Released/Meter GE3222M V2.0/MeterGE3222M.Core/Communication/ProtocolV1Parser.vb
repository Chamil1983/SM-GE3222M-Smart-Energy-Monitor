Option Strict On

' SM-GE3222M V2.0 - Protocol V1 Parser
' Parses V1 Tag:Value protocol format

Imports System.Text.RegularExpressions
Imports MeterGE3222M.Core.Models

Namespace Communication
    ''' <summary>
    ''' V1 Tag:Value protocol parser
    ''' Parses tags like AE1, AP1-AP10, BE1-BE5, BP1-BP10, etc.
    ''' Format: TAG:VALUE or TAGVALUE (e.g., "AE1:123.45" or "AE1123.45")
    ''' </summary>
    Public Class ProtocolV1Parser
        ' Regex pattern to match V1 tags
        ' Matches: TAG (2 letters + 1-2 digits) followed by value (number with optional sign and decimal)
        Private Shared ReadOnly TagPattern As New Regex("([A-Z]{2}\d{1,2})([-.\d]+)", RegexOptions.Compiled Or RegexOptions.Multiline)
        
        ''' <summary>
        ''' Parse V1 protocol data
        ''' </summary>
        ''' <param name="data">Raw V1 text data</param>
        ''' <returns>Parsed MeterSnapshot</returns>
        Public Function Parse(data As String) As MeterSnapshot
            If String.IsNullOrWhiteSpace(data) Then
                Return New MeterSnapshot() With {.Valid = False}
            End If
            
            ' Parse tags and values into dictionary
            Dim tagValueMap = ParseTags(data)
            
            If tagValueMap.Count = 0 Then
                Return New MeterSnapshot() With {.Valid = False}
            End If
            
            ' Convert tag-value map to MeterSnapshot
            Return MeterSnapshot.FromTagValueMap(tagValueMap)
        End Function
        
        ''' <summary>
        ''' Parse tags and values from V1 data
        ''' </summary>
        ''' <param name="data">Raw V1 text data</param>
        ''' <returns>Dictionary of tag -> value pairs</returns>
        Public Function ParseTags(data As String) As Dictionary(Of String, Single)
            Dim tagValueMap As New Dictionary(Of String, Single)()
            
            Try
                ' Find all matches
                Dim matches = TagPattern.Matches(data)
                
                For Each match As Match In matches
                    If match.Success AndAlso match.Groups.Count >= 3 Then
                        Dim tag = match.Groups(1).Value
                        Dim valueStr = match.Groups(2).Value
                        
                        ' Try parse value
                        Dim value As Single
                        If Single.TryParse(valueStr, Globalization.NumberStyles.Float, Globalization.CultureInfo.InvariantCulture, value) Then
                            ' Add or update tag value
                            tagValueMap(tag) = value
                        End If
                    End If
                Next
                
            Catch ex As Exception
                ' Return empty dictionary on parse error
            End Try
            
            Return tagValueMap
        End Function
        
        ''' <summary>
        ''' Try parse V1 protocol data
        ''' </summary>
        ''' <param name="data">Raw V1 text data</param>
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
        ''' Extract specific tag value from V1 data
        ''' </summary>
        ''' <param name="data">Raw V1 text data</param>
        ''' <param name="tag">Tag to extract (e.g., "AE1", "AP1")</param>
        ''' <param name="value">Extracted value (output)</param>
        ''' <returns>True if tag found and extracted</returns>
        Public Function TryExtractTag(data As String, tag As String, ByRef value As Single) As Boolean
            If String.IsNullOrWhiteSpace(data) OrElse String.IsNullOrWhiteSpace(tag) Then
                Return False
            End If
            
            Try
                ' Create pattern for specific tag
                Dim pattern = $"({tag})([-.\d]+)"
                Dim regex As New Regex(pattern, RegexOptions.IgnoreCase)
                Dim match = regex.Match(data)
                
                If match.Success AndAlso match.Groups.Count >= 3 Then
                    Dim valueStr = match.Groups(2).Value
                    Return Single.TryParse(valueStr, Globalization.NumberStyles.Float, Globalization.CultureInfo.InvariantCulture, value)
                End If
                
            Catch ex As Exception
                ' Return false on error
            End Try
            
            Return False
        End Function
        
        ''' <summary>
        ''' Get list of all tags found in data
        ''' </summary>
        ''' <param name="data">Raw V1 text data</param>
        ''' <returns>List of tag names</returns>
        Public Function GetTagList(data As String) As List(Of String)
            Dim tags As New List(Of String)()
            
            If String.IsNullOrWhiteSpace(data) Then
                Return tags
            End If
            
            Try
                Dim matches = TagPattern.Matches(data)
                
                For Each match As Match In matches
                    If match.Success AndAlso match.Groups.Count >= 2 Then
                        Dim tag = match.Groups(1).Value
                        If Not tags.Contains(tag) Then
                            tags.Add(tag)
                        End If
                    End If
                Next
                
            Catch ex As Exception
                ' Return empty list on error
            End Try
            
            Return tags
        End Function
        
        ''' <summary>
        ''' Validate V1 protocol data format
        ''' </summary>
        ''' <param name="data">Raw V1 text data</param>
        ''' <returns>True if data appears to be valid V1 format</returns>
        Public Function ValidateFormat(data As String) As Boolean
            If String.IsNullOrWhiteSpace(data) Then
                Return False
            End If
            
            Try
                ' Check if data contains at least one valid tag
                Dim matches = TagPattern.Matches(data)
                Return matches.Count > 0
                
            Catch ex As Exception
                Return False
            End Try
        End Function
    End Class
End Namespace
