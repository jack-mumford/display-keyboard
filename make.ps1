
$allArgs = $PsBoundParameters.Values + $args
python $PSScriptRoot\make.py $allArgs
