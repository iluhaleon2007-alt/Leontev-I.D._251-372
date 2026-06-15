$clients = @()
$port = 33333
$hostAddr = "127.0.0.1"

# Подключаем 5 клиентов
for ($i = 1; $i -le 5; $i++) {
    $client = New-Object System.Net.Sockets.TcpClient
    $client.Connect($hostAddr, $port)
    $clients += $client
    Start-Sleep -Milliseconds 300
    $stream = $client.GetStream()
    $buffer = New-Object byte[] 1024
    $bytes = $stream.Read($buffer, 0, 1024)
    $msg = [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytes)
    Write-Host "=== CLIENT $i ===" $msg
    Write-Host ""
}

Start-Sleep -Milliseconds 500

# Клиент 1 отправляет догадку 50
$stream1 = $clients[0].GetStream()
$writer1 = New-Object System.IO.StreamWriter($stream1)
$writer1.AutoFlush = $true
$writer1.WriteLine("50")
Start-Sleep -Milliseconds 300

$buffer = New-Object byte[] 1024
$bytes = $stream1.Read($buffer, 0, 1024)
Write-Host "=== CLIENT 1 === отправил 50, ответ:" [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytes)

# Клиент 2 отправляет догадку 75
$stream2 = $clients[1].GetStream()
$writer2 = New-Object System.IO.StreamWriter($stream2)
$writer2.AutoFlush = $true
$writer2.WriteLine("75")
Start-Sleep -Milliseconds 300

$buffer = New-Object byte[] 1024
$bytes = $stream2.Read($buffer, 0, 1024)
Write-Host "=== CLIENT 2 === отправил 75, ответ:" [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytes)

# Клиент 3 отправляет догадку 25
$stream3 = $clients[2].GetStream()
$writer3 = New-Object System.IO.StreamWriter($stream3)
$writer3.AutoFlush = $true
$writer3.WriteLine("25")
Start-Sleep -Milliseconds 300

$buffer = New-Object byte[] 1024
$bytes = $stream3.Read($buffer, 0, 1024)
Write-Host "=== CLIENT 3 === отправил 25, ответ:" [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytes)

# Клиент 4 отправляет догадку 37
$stream4 = $clients[3].GetStream()
$writer4 = New-Object System.IO.StreamWriter($stream4)
$writer4.AutoFlush = $true
$writer4.WriteLine("37")
Start-Sleep -Milliseconds 300

$buffer = New-Object byte[] 1024
$bytes = $stream4.Read($buffer, 0, 1024)
Write-Host "=== CLIENT 4 === отправил 37, ответ:" [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytes)

# Клиент 5 отправляет догадку 12
$stream5 = $clients[4].GetStream()
$writer5 = New-Object System.IO.StreamWriter($stream5)
$writer5.AutoFlush = $true
$writer5.WriteLine("12")
Start-Sleep -Milliseconds 300

$buffer = New-Object byte[] 1024
$bytes = $stream5.Read($buffer, 0, 1024)
Write-Host "=== CLIENT 5 === отправил 12, ответ:" [System.Text.Encoding]::UTF8.GetString($buffer, 0, $bytes)

# Закрываем клиентов
for ($i = 0; $i -lt 5; $i++) {
    $clients[$i].Close()
}

Write-Host ""
Write-Host "=== ТЕСТ ЗАВЕРШЁН ==="
