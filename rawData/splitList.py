"""
Splits the list of possible tokens on the cards into a list to be used tesseract.
"""

__author__      = "Clayton Gearhart"
__copyright__ = "Copyright 2023 Clayton Gearhart"
__license__     = "GNU Lesser General Public License v3.0"
__version__    = "1.0"

import numpy as np
import pandas as pd

# mlist = combinedList = pd.read_csv("combined.csv", usecols=["First Name","Middle Name","Last Name","Suffix Name", "Business Address 1","Business Address 2","Business City","Business State","Business Zip"], dtype="str")
mlist = combinedList = pd.read_csv("michiganList.csv", usecols=["person_fname","person_lname","person_suffix_type_name","address_line1","address_line2","city","state_province_name","postal_code","postal","plus 4 code"], dtype="str")


flat = mlist.to_numpy(dtype="str").flatten()
flat = [item for current in flat for item in current.split()]
flat = list(set(flat))
with open("inWords.txt", mode="a") as file:
    for word in flat:
        file.write(f"{str(word)},")