#!/bin/bash
iconsPath=${1}
qrcPath=${2}
namesOfRequiredIcons=($(grep --only-matching '\"[a-z-]*\"' $iconsPath | sort -u | tr -d \"))

HERE="$(dirname "$(readlink -f "${0}")")"

# names of icons that should be included in the rcc file but weren't added automatically
namesOfRequiredIconsWhiteList=($(cat $HERE/whitelist))
# names of icons that shouldn't be included in rcc file but were added automatically
namesOfRequiredIconsBlackList=($(cat $HERE/blacklist))

for i in ${!namesOfRequiredIconsBlackList[@]}; do
  for j in ${!namesOfRequiredIcons[@]}; do
    if [[ ${namesOfRequiredIcons[j]} == ${namesOfRequiredIconsBlackList[i]} ]]; then
      unset 'namesOfRequiredIcons[j]'
    fi
  done
done

for i in ${!namesOfRequiredIconsWhiteList[@]}; do
  namesOfRequiredIcons+=(${namesOfRequiredIconsWhiteList[i]})
done

namesOfAvailableIcons=($(grep --only-matching '/[0-9A-Za-z_+.-]*\.svg' $qrcPath | sort -u | tr -d / | rev | cut -d . -f2- | rev))

namesOfMissingIcons=()
for nameOfRequiredIcon in ${namesOfRequiredIcons[@]}; do
  isIconFound=false
  for nameOfAvailableIcon in ${namesOfAvailableIcons[@]}; do
    if [ $nameOfAvailableIcon = $nameOfRequiredIcon ]; then
      isIconFound=true
    fi
  done

  if [ $isIconFound = false ]; then
    namesOfMissingIcons+=($nameOfRequiredIcon)
  fi
done

namesOfNotRequiredIcons=()
for nameOfAvailableIcon in ${namesOfAvailableIcons[@]}; do
  isIconFound=false
  for nameOfRequiredIcon in ${namesOfRequiredIcons[@]}; do
    if [ $nameOfAvailableIcon = $nameOfRequiredIcon ]; then
      isIconFound=true
      break
    fi
  done

  if [ $isIconFound = false ]; then
    namesOfNotRequiredIcons+=($nameOfAvailableIcon)
  fi
done

if [ ${#namesOfMissingIcons[@]} -gt 0 ]; then
echo "Following icon names are missing in breeze theme: "
  for nameOfMissingIcon in ${namesOfMissingIcons[@]}; do
    echo "  $nameOfMissingIcon"
  done
fi

qrcFileContent=$(cat $qrcPath)
qrcFileContent=$(echo "$qrcFileContent" | sed '/@2x/d')
for nameOfNotRequiredIcon in ${namesOfNotRequiredIcons[@]}; do
    qrcFileContent=$(echo "$qrcFileContent" | sed '/\/'$nameOfNotRequiredIcon'.svg</d')
done
echo "$qrcFileContent" > $qrcPath

namesOfLeftIcons=($(grep --only-matching '/[0-9A-Za-z_+.-]*\.svg' $qrcPath | sort -u | tr -d / | rev | cut -d . -f2- | rev))

echo "Number of icons available in breeze theme: ${#namesOfAvailableIcons[@]}"
echo "Number of icons required by KMyMoney: ${#namesOfRequiredIcons[@]}"
echo "Number of icons not-required in breeze theme: ${#namesOfNotRequiredIcons[@]}"
echo "Number of icons missing in breeze theme: ${#namesOfMissingIcons[@]}"
echo "Number of icons left in breeze theme: ${#namesOfLeftIcons[@]}"