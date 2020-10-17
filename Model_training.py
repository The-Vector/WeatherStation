#importing all of the necessary libraries, most have to be installed via pip/downloading
import scipy 
import numpy as np
import matplotlib as plt
import pandas
import sklearn
import emlearn

from pandas import read_csv
from pandas.plotting import scatter_matrix
from matplotlib import pyplot
from sklearn.model_selection import train_test_split
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import StratifiedKFold
from sklearn.metrics import classification_report
from sklearn.metrics import confusion_matrix
from sklearn.metrics import accuracy_score
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC

#select the data file, csv = comma separated values
url = 'WeatherDataTest.csv'
#names for each columns
names = ["Rainfall (mm)","Temperature (°C)","relative humidity (%)","Is raining"]
dataset = read_csv(url, names=names)

#make an array with all of the data
array = dataset.values

#x values represent what the machin elearning algorithms is inputted
X = array[:, 0:3]
#y value represents the outcome, e.g if there is rain, temperature is 10 degrees and humidity is 80%
#the y value would be "raining" as an example
Y = array[:, 3]

#split data into training and testing 
x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size = 0.2, random_state = 1)


#create a naive bayes model
nb_model = DecisionTreeClassifier()

#train the model withthe training data
nb_model.fit(x_train, y_train)

#get the model to give it's own prediction on the test data
predictions = nb_model.predict(x_test)

#print out the accuracy of the prediction based on the real y value
print(accuracy_score(y_test, predictions))
print(confusion_matrix(y_test, predictions))
print(classification_report(y_test, predictions))

#convert the model to an arduino readable format.
filename = 'RAIN.h'
cmodel = emlearn.convert(nb_model)
code = cmodel.save(file=filename) # create the Rain Analysis Intelligence Network, aka RAIN