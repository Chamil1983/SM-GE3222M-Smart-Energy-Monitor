Option Strict On

' SM-GE3222M V2.0 - Protocol V2 Parser
' Parses V2 JSON protocol format

Imports System.Text.Json
Imports MeterGE3222M.Core.Models

Namespace Communication
    ''' <summary>
    ''' V2 JSON protocol parser
    ''' Parses JSON data into MeterSnapshot using System.Text.Json
    ''' </summary>
    Public Class ProtocolV2Parser
        Private ReadOnly _jsonOptions As JsonSerializerOptions
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        Public Sub New()
            ' Configure JSON serializer options
            _jsonOptions = New JsonSerializerOptions() With {
                .PropertyNameCaseInsensitive = True,
                .AllowTrailingCommas = True,
                .ReadCommentHandling = JsonCommentHandling.Skip
            }
        End Sub
        
        ''' <summary>
        ''' Parse V2 JSON protocol data
        ''' </summary>
        ''' <param name="jsonString">JSON string from meter</param>
        ''' <returns>Parsed MeterSnapshot</returns>
        Public Function Parse(jsonString As String) As MeterSnapshot
            If String.IsNullOrWhiteSpace(jsonString) Then
                Return New MeterSnapshot() With {.Valid = False}
            End If
            
            Try
                ' Deserialize JSON to MeterSnapshot
                Dim snapshot = JsonSerializer.Deserialize(Of MeterSnapshot)(jsonString, _jsonOptions)
                
                If snapshot Is Nothing Then
                    Return New MeterSnapshot() With {.Valid = False}
                End If
                
                ' Mark as valid
                snapshot.Valid = True
                
                Return snapshot
                
            Catch ex As JsonException
                ' JSON parse error
                Return New MeterSnapshot() With {.Valid = False}
            Catch ex As Exception
                ' Other error
                Return New MeterSnapshot() With {.Valid = False}
            End Try
        End Function
        
        ''' <summary>
        ''' Try parse V2 JSON protocol data
        ''' </summary>
        ''' <param name="jsonString">JSON string from meter</param>
        ''' <param name="snapshot">Parsed MeterSnapshot (output)</param>
        ''' <returns>True if parsed successfully</returns>
        Public Function TryParse(jsonString As String, ByRef snapshot As MeterSnapshot) As Boolean
            Try
                snapshot = Parse(jsonString)
                Return snapshot IsNot Nothing AndAlso snapshot.Valid
            Catch ex As Exception
                snapshot = New MeterSnapshot() With {.Valid = False}
                Return False
            End Try
        End Function
        
        ''' <summary>
        ''' Serialize MeterSnapshot to JSON string
        ''' </summary>
        ''' <param name="snapshot">MeterSnapshot to serialize</param>
        ''' <param name="indented">Use indented formatting</param>
        ''' <returns>JSON string</returns>
        Public Function Serialize(snapshot As MeterSnapshot, Optional indented As Boolean = False) As String
            If snapshot Is Nothing Then
                Throw New ArgumentNullException(NameOf(snapshot))
            End If
            
            Try
                Dim options As New JsonSerializerOptions() With {
                    .PropertyNameCaseInsensitive = True,
                    .WriteIndented = indented
                }
                
                Return JsonSerializer.Serialize(snapshot, options)
                
            Catch ex As Exception
                Throw New InvalidOperationException("Failed to serialize MeterSnapshot", ex)
            End Try
        End Function
        
        ''' <summary>
        ''' Try serialize MeterSnapshot to JSON string
        ''' </summary>
        ''' <param name="snapshot">MeterSnapshot to serialize</param>
        ''' <param name="jsonString">JSON string (output)</param>
        ''' <param name="indented">Use indented formatting</param>
        ''' <returns>True if serialized successfully</returns>
        Public Function TrySerialize(snapshot As MeterSnapshot, ByRef jsonString As String, Optional indented As Boolean = False) As Boolean
            Try
                jsonString = Serialize(snapshot, indented)
                Return True
            Catch ex As Exception
                jsonString = String.Empty
                Return False
            End Try
        End Function
        
        ''' <summary>
        ''' Validate V2 JSON format
        ''' </summary>
        ''' <param name="jsonString">JSON string to validate</param>
        ''' <returns>True if valid JSON</returns>
        Public Function ValidateFormat(jsonString As String) As Boolean
            If String.IsNullOrWhiteSpace(jsonString) Then
                Return False
            End If
            
            Try
                ' Try to parse as JSON document
                Using document = JsonDocument.Parse(jsonString)
                    ' Check if it's an object (not array or primitive)
                    Return document.RootElement.ValueKind = JsonValueKind.Object
                End Using
                
            Catch ex As JsonException
                Return False
            Catch ex As Exception
                Return False
            End Try
        End Function
        
        ''' <summary>
        ''' Extract specific property from JSON string without full deserialization
        ''' </summary>
        ''' <param name="jsonString">JSON string</param>
        ''' <param name="propertyName">Property name to extract</param>
        ''' <returns>Property value as string, or Nothing if not found</returns>
        Public Function ExtractProperty(jsonString As String, propertyName As String) As String
            If String.IsNullOrWhiteSpace(jsonString) OrElse String.IsNullOrWhiteSpace(propertyName) Then
                Return Nothing
            End If
            
            Try
                Using document = JsonDocument.Parse(jsonString)
                    Dim root = document.RootElement
                    
                    If root.TryGetProperty(propertyName, Nothing) Then
                        Dim prop As JsonProperty
                        If root.TryGetProperty(propertyName, prop) Then
                            Return prop.Value.ToString()
                        End If
                    End If
                End Using
                
            Catch ex As Exception
                ' Return nothing on error
            End Try
            
            Return Nothing
        End Function
        
        ''' <summary>
        ''' Get list of all property names in JSON
        ''' </summary>
        ''' <param name="jsonString">JSON string</param>
        ''' <returns>List of property names</returns>
        Public Function GetPropertyNames(jsonString As String) As List(Of String)
            Dim properties As New List(Of String)()
            
            If String.IsNullOrWhiteSpace(jsonString) Then
                Return properties
            End If
            
            Try
                Using document = JsonDocument.Parse(jsonString)
                    Dim root = document.RootElement
                    
                    If root.ValueKind = JsonValueKind.Object Then
                        For Each prop In root.EnumerateObject()
                            properties.Add(prop.Name)
                        Next
                    End If
                End Using
                
            Catch ex As Exception
                ' Return empty list on error
            End Try
            
            Return properties
        End Function
    End Class
End Namespace
