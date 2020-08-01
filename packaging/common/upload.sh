#!/bin/bash

#  *
#  * Copyright 2020  Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
#  *
#  * This program is free software; you can redistribute it and/or
#  * modify it under the terms of the GNU General Public License as
#  * published by the Free Software Foundation; either version 2 of
#  * the License, or (at your option) any later version.
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  *

set +x # Do not leak information

# Requirements:
# 1. Global environment variable GITHUB_TOKEN
# 2. Permissions for operating GitHub through API: repo:status repo_deployment public_repo

# Usage:
# ./upload.sh "KMyMoneyNEXT for Linux" "/home/travis/image/KMyMoneyNEXT-5.0.80-e05a5b2-x86_64.AppImage"

# Bahaviour:
# 1. creates ${tag_name} with ${displayed_name} if it doesn't exist on GitHub
# 2. if exist, then it removes GitHub asset by passed label e.g. "KMyMoneyNEXT for Linux"
# 3. uploads new GitHub asset
# 4. leaves other assets intact

asset_label=$1
asset_file=$2

echo "About to upload a file '$asset_file' under a label '$asset_label' on GitHub"

IFS=$'\n'

if [ ! -z ${TRAVIS+x} ]; then
  echo "Uploading from CI Travis..."
  echo "TRAVIS_REPO_SLUG: ${TRAVIS_REPO_SLUG}"
  owner_name="${TRAVIS_REPO_SLUG%/*}"
  repo_name="${TRAVIS_REPO_SLUG#*/}"
elif [ ! -z ${APPVEYOR+x} ]; then
  echo "Uploading from Appveyor..."
  echo "APPVEYOR_REPO_NAME: $APPVEYOR_REPO_NAME}"
  owner_name="${APPVEYOR_REPO_NAME%/*}"
  repo_name="${APPVEYOR_REPO_NAME#*/}"
else
  echo "No repository information. Exiting..."
  exit
fi

echo "Owner name: $owner_name"
echo "Repository name: $repo_name"

authorization="Authorization: token ${GITHUB_TOKEN}"
gihub_url="https://api.github.com"

releases_url="$gihub_url/repos/${owner_name}/${repo_name}/releases"
tags_url="${releases_url}/tags"
assets_url="${releases_url}/assets"

displayed_name="Continuous releases"
tag_name="continuous_releases"

valueFromKey () {
  # $1 - json,
  # $2 - argument name
  # retval - argument value
  
  echo "$1" | grep "$2" | sed -E s/.*\"\("$2"\)\"": "\"\(.*\)\".*/"\2"/
}

tagId () {
  # $1 - tag name
  retval=$(curl --request GET \
              --header "${authorization}" \
              "${tags_url}/$1")

  if [[ ! -z $(echo "$retval" | grep "Not Found") ]]; then
    echo ""
    return
  fi
    
  id=$(echo "$retval" | grep "url" | head -n 1)
  id=$(valueFromKey "$id" "url")
  id=${id##*/}
  echo "$id"
}

createTag () {
  # $1 - tag name
  # $2 - displayed name

#do not inted this because variable "request" would be bad constructed
request=$(cat << END_OF_REQUEST
{
  "tag_name": "$1",
  "target_commitish": "",
  "name": "$2",
  "body": "",
  "draft": false,
  "prerelease": true
}
END_OF_REQUEST
)
  
  retval=$(curl --request POST \
                --header "${authorization}" \
                --data "${request}" \
                "${releases_url}")
  echo "$retval"
}

deleteTag () {
  # $1 - tag name

  tagId=$(tagId "$1")
  if [[ -z "$tagId" ]]; then
    return
  fi

  retval=$(curl --request DELETE \
                --header "${authorization}" \
                --data "${request}" \
                "${releases_url}/${tagId}")

  retval+=$(curl --request DELETE \
                --header "${authorization}" \
                --data "${request}" \
                "$gihub_url/repos/${owner_name}/${repo_name}/git/refs/tags/$1")
  echo "$retval"
}

updateTag () {
  # $1 - tag name
  # $2 - displayed name
  # $3 - body

  tagId=$(tagId "$1")
  if [[ -z "$tagId" ]]; then
    return
  fi
  
#do not inted this because variable "request" would be bad constructed
request=$(cat << END_OF_REQUEST
{
  "tag_name": "$1",
  "target_commitish": "master",
  "name": "$2",
  "body": "$3",
  "draft": false,
  "prerelease": true
}
END_OF_REQUEST
)

  retval=$(curl --request PATCH \
                --header "${authorization}" \
                --data "${request}" \
                "${releases_url}/${tagId}")
  echo "$retval"
}

assets () {
  # $1 - tag name
  tagId=$(tagId "$1")
  if [[ -z "$tagId" ]]; then
    return
  fi
  
  retval=$(curl --request GET \
                --header "${authorization}" \
                "${releases_url}/${tagId}/assets")
  if [[ ${#retval} -lt 5 ]]; then
    echo ""
    return
  fi
  
  echo "$retval"
}

removeAsset () {
  # $1 - asset id
  retval=$(curl --request DELETE \
                --header "${authorization}" \
                "$assets_url/$1")
  echo "${retval}"
}

addAsset () {
  # $1 - tag name
  # $2 - filepath to upload
  # $3 - filelabel
  tag_info=$(curl --request GET \
                  --header "${authorization}" \
                  "${tags_url}/$1")

  upload_url=$(valueFromKey "$tag_info" "upload_url")
  upload_url=$(echo ${upload_url%%\{*})

  file_base_name="$(basename "${2}")"

  retval=$(curl --header "${authorization}" \
                --header "Content-Type: application/octet-stream" \
                --data-binary @$2 \
                "$upload_url?name=$file_base_name&label=$3")
  echo "${retval}"
}

assetNames () {
  # $1 - unparsed assets
  arr=(`echo "${1}" | grep "name"`)
    
  for (( i = 0; i < ${#arr[@]} ; i++ ))
  do
    arr[$i]=$(valueFromKey "${arr[$i]}" "name")
  done
  
  echo "${arr[*]}"
}

assetLabels () {
  # $1 - unparsed assets
  arr=(`echo "${1}" | grep "label"`)
  
  for (( i = 0; i < ${#arr[@]} ; i++ ))
  do
    arr[$i]=$(valueFromKey "${arr[$i]}" "label")
  done
  
  echo "${arr[*]}"
}

assetIds () {
  # $1 - unparsed assets
  arr=(`echo "${1}" | grep "${assets_url}"`)
  
  for (( i = 0; i < ${#arr[@]} ; i++ ))
  do
    arr[$i]=$(valueFromKey "${arr[$i]}" "url")
    id=${arr[$i]}
    arr[$i]=${id##*/}
  done
  
  echo "${arr[*]}"
}

assetCreationTime () {
  # $1 - unparsed assets
  arr=(`echo "${1}" | grep "created_at"`)
  
  for (( i = 0; i < ${#arr[@]} ; i++ ))
  do
    arr[$i]=$(valueFromKey "${arr[$i]}" "created_at") # 2020-04-30T12:21:47Z
    # ${arr[$i]:0:-4} # doesn't work on macOS
    arr[$i]=${arr[$i]%????} # 2020-04-30T12:21
    arr[$i]="built on ${arr[$i]/T/ at }" # build on 2020-04-30 at 12:21
  done
  
  echo "${arr[*]}"
}

assetDownloadUrl () {
  # $1 - unparsed assets
  arr=(`echo "${1}" | grep "browser_download_url"`)
  
  for (( i = 0; i < ${#arr[@]} ; i++ ))
  do
    arr[$i]=$(valueFromKey "${arr[$i]}" "browser_download_url")
  done
  
  echo "${arr[*]}"
}

id=$(tagId "${tag_name}")

if [[ -z "$id" ]]; then
  echo "Creating tag name '${tag_name}'..."
  createTag "${tag_name}" "${displayed_name}"
else
  assets_unparsed=$(assets "$tag_name")
  if [[ ! -z "$assets_unparsed" ]]; then

    remove_all_assets=true
    asset_download_urls=($(assetDownloadUrl "${assets_unparsed}"))
    KMYMONEY_VERSION=$(echo "$asset_file" | cut -d'-' -f2- | rev | cut -d'-' -f2- | rev)
    for (( i = 0; i < ${#asset_download_urls[@]} ; i++ ))
      do
        if [[ "${asset_download_urls[i]}" == *"${KMYMONEY_VERSION}"* ]]; then
          remove_all_assets=false
          break
        fi
      done

    if [ "$remove_all_assets" = true ]; then
      echo "Recreating tag name '${tag_name}'..."
      deleteTag "${tag_name}"
      createTag "${tag_name}" "${displayed_name}"
    else
      asset_labels=($(assetLabels "${assets_unparsed}"))
      asset_ids=($(assetIds "${assets_unparsed}"))
      asset_label_unescaped=$(echo "$asset_label" | sed s/"%20"/" "/g)

      for (( i = 0; i < ${#asset_labels[@]} ; i++ ))
        do
          if [[ "${asset_labels[$i]}" == "${asset_label_unescaped}" ]]; then
            echo "Removing asset label '${asset_labels[$i]}'..."
            removeAsset "${asset_ids[$i]}"
          fi
        done
    fi
  fi
fi
  
echo "Adding asset file '${asset_file}'..."
addAsset "$tag_name" "${asset_file}" "${asset_label}"

echo "Updating body of the tag..."
assets_unparsed=$(assets "$tag_name")
asset_creation_times=($(assetCreationTime "${assets_unparsed}"))
asset_download_urls=($(assetDownloadUrl "${assets_unparsed}"))
asset_labels=($(assetLabels "${assets_unparsed}"))

for (( i = 0; i < ${#asset_labels[@]} ; i++ ))
do
  tag_body+="<a href=${asset_download_urls[i]}>${asset_labels[i]}</a> ${asset_creation_times[i]}.<br>"
  
  if [[ "${asset_labels[i]}" == *"Linux"* ]]; then
    tag_body_requirements+="&nbsp;&nbsp;-Ubuntu 16.04 (or equivalent)<br>"
  elif [[ "${asset_labels[i]}" == *"macOS"* ]]; then
    tag_body_requirements+="&nbsp;&nbsp;-macOS High Sierra<br>"
  elif [[ "${asset_labels[i]}" == *"Windows"* ]]; then
    tag_body_requirements+="&nbsp;&nbsp;-Microsoft Windows 7<br>"
  fi
done

if [[ -z "${tag_body}" ]]; then
  tag_body="Currently there are some issues with builds."
else
  tag_body+="<br>minimum system requirements:<br>${tag_body_requirements}"
fi

tag_body=${tag_body%<br>}
updateTag "${tag_name}" "${displayed_name}" "${tag_body}"